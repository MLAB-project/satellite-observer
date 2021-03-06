/* -*- c++ -*- */
/*
 * Copyright 2014 Carlos Alberto Ruiz Naranjo
 * carlosruiznaranjo@gmail.com
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "noaa_analog_decoder_f_impl.h"

#include <boost/math/special_functions/round.hpp>
#include <gnuradio/blocks/wavfile.h>

#include <stdexcept>
#include <climits>
#include <cstring>
#include <cmath>
#include <fcntl.h>

#include "decode.h"



// win32 (mingw/msvc) specific
#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef O_BINARY
#define OUR_O_BINARY O_BINARY
#else
#define OUR_O_BINARY 0
#endif

// should be handled via configure
#ifdef O_LARGEFILE
#define OUR_O_LARGEFILE O_LARGEFILE
#else
#define OUR_O_LARGEFILE 0
#endif


namespace gr {
  namespace sat_observer {

    noaa_analog_decoder_f::sptr
    noaa_analog_decoder_f::make(std::string filename, std::string mode, int noaaNumber)
    {
      return gnuradio::get_initial_sptr
        (new noaa_analog_decoder_f_impl(filename, mode, noaaNumber));
    }

    /*
     * The private constructor
     */
    noaa_analog_decoder_f_impl::noaa_analog_decoder_f_impl(std::string filename, std::string mode, int noaaNumber)
      : gr::sync_block("noaa_analog_decoder_f",
              gr::io_signature::make(1,1, sizeof(float)),
              gr::io_signature::make(0, 0, 0)),
                d_sample_rate(11025), d_nchans(1),
                    d_fp(0), d_new_fp(0), d_updated(false)
    {
        set_file(filename);
        set_mode(mode);
        set_satelliteNumber(noaaNumber);

        int bits_per_sample=16;
        d_bytes_per_sample = 16 / 8;
        d_bytes_per_sample_new = d_bytes_per_sample;

        if(!open(filename.c_str())) {
            throw std::runtime_error("can't open file");
        }

        if(bits_per_sample == 8) {
            d_max_sample_val = 0xFF;
            d_min_sample_val = 0;
            d_normalize_fac  = d_max_sample_val/2;
            d_normalize_shift = 1;
        }
        else {
            d_max_sample_val = 0x7FFF;
            d_min_sample_val = -0x7FFF;
            d_normalize_fac  = d_max_sample_val;
            d_normalize_shift = 0;
            if(bits_per_sample != 16) {
                fprintf(stderr, "Invalid bits per sample value requested, using 16");
            }
        }
    }

    /*
     * Our virtual destructor.
     */
    noaa_analog_decoder_f_impl::~noaa_analog_decoder_f_impl()
    {
      stop();
    }


    short int
    noaa_analog_decoder_f_impl::convert_to_short(float sample)
    {
      sample += d_normalize_shift;
      sample *= d_normalize_fac;
      if(sample > d_max_sample_val) {
	sample = d_max_sample_val;
      }
      else if(sample < d_min_sample_val) {
	sample = d_min_sample_val;
      }

      return (short int)boost::math::iround(sample);
    }



    void
    noaa_analog_decoder_f_impl::do_update()
    {
      if(!d_updated) {
	return;
      }

      if(d_fp) {
	close_wav();
      }

      d_fp = d_new_fp;                    // install new file pointer
      d_new_fp  = 0;
      d_sample_count = 0;
      d_bytes_per_sample = d_bytes_per_sample_new;

      if(d_bytes_per_sample == 1) {
	d_max_sample_val = UCHAR_MAX;
	d_min_sample_val = 0;
	d_normalize_fac  = d_max_sample_val/2;
	d_normalize_shift = 1;
      }
      else if(d_bytes_per_sample == 2) {
	d_max_sample_val = SHRT_MAX;
	d_min_sample_val = SHRT_MIN;
	d_normalize_fac  = d_max_sample_val;
	d_normalize_shift = 0;
      }

      d_updated = false;
    }

    void
    noaa_analog_decoder_f_impl::close_wav()
    {
      unsigned int byte_count = d_sample_count * d_bytes_per_sample;

      gr::blocks::wavheader_complete(d_fp, byte_count);

      fclose(d_fp);
      d_fp = NULL;
      decode(get_file(),get_mode(),get_satelliteNumber());
    }


    bool noaa_analog_decoder_f_impl::stop()
    {
      if(d_new_fp) {
        fclose(d_new_fp);
        d_new_fp = NULL;
      }
      close();

      return true;
    }

    void
    noaa_analog_decoder_f_impl::close()
    {
      gr::thread::scoped_lock guard(d_mutex);

      if(!d_fp)
	return;

      close_wav();
    }



    bool
    noaa_analog_decoder_f_impl::open(const char* filename)
    {
        gr::thread::scoped_lock guard(d_mutex);

        // we use the open system call to get access to the O_LARGEFILE flag.
        int fd;
        if((fd = ::open(filename,
            O_WRONLY|O_CREAT|O_TRUNC|OUR_O_LARGEFILE|OUR_O_BINARY,
                0664)) < 0) {
            perror(filename);
            return false;
        }

        if(d_new_fp) {    // if we've already got a new one open, close it
            fclose(d_new_fp);
            d_new_fp = 0;
        }

        if((d_new_fp = fdopen (fd, "wb")) == NULL) {
            perror(filename);
            ::close(fd);  // don't leak file descriptor if fdopen fails.
            return false;
        }
        d_updated = true;

        if(!gr::blocks::wavheader_write(d_new_fp,
            d_sample_rate,
                d_nchans,
                    d_bytes_per_sample_new)) {
            fprintf(stderr, "[%s] could not write to WAV file\n", __FILE__);
            exit(-1);
        }
      return true;
    }


    void
    noaa_analog_decoder_f_impl::set_sample_rate(unsigned int sample_rate)
    {
      gr::thread::scoped_lock guard(d_mutex);
      d_sample_rate = sample_rate;
    }


    void
    noaa_analog_decoder_f_impl::set_bits_per_sample(int bits_per_sample)
    {
      gr::thread::scoped_lock guard(d_mutex);
      if(bits_per_sample == 8 || bits_per_sample == 16) {
	d_bytes_per_sample_new = bits_per_sample / 8;
      }
    }

    void
    noaa_analog_decoder_f_impl::set_file(std::string f)
    {
      file = f;
    }

    void
    noaa_analog_decoder_f_impl::set_mode(std::string m)
    {
      mode = m;
    }

    void
    noaa_analog_decoder_f_impl::set_satelliteNumber(int sN)
    {
      satelliteNumber = sN;
    }

    std::string
    noaa_analog_decoder_f_impl::get_file()
    {
      return file;
    }

    std::string
    noaa_analog_decoder_f_impl::get_mode()
    {
      return mode;
    }

    int
    noaa_analog_decoder_f_impl::get_satelliteNumber()
    {
      return satelliteNumber;
    }

    int
    noaa_analog_decoder_f_impl::bits_per_sample()
    {
      return d_bytes_per_sample_new;
    }

    unsigned int
    noaa_analog_decoder_f_impl::sample_rate()
    {
      return d_sample_rate;
    }





    int
    noaa_analog_decoder_f_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        float **in = (float**)&input_items[0];
        int n_in_chans = input_items.size();

        short int sample_buf_s;

        int nwritten;

        gr::thread::scoped_lock guard(d_mutex);    // hold mutex for duration of this block
        do_update();      // update: d_fp is reqd
        if(!d_fp)         // drop output on the floor
            return noutput_items;

        for(nwritten = 0; nwritten < noutput_items; nwritten++) {
            for(int chan = 0; chan < d_nchans; chan++) {
            // Write zeros to channels which are in the WAV file
            // but don't have any inputs here
            if(chan < n_in_chans) {
                sample_buf_s =
                convert_to_short(in[chan][nwritten]);
            }
            else {
                sample_buf_s = 0;
            }

            gr::blocks::wav_write_sample(d_fp, sample_buf_s, d_bytes_per_sample);

            if(feof(d_fp) || ferror(d_fp)) {
                fprintf(stderr, "[%s] file i/o error\n", __FILE__);
                close();
                exit(-1);
            }
            d_sample_count++;
        }
      }
      return nwritten;
    }

  } /* namespace sat_observer */
} /* namespace gr */

