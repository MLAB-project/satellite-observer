/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_SAT_OBSERVER_NOAA_ANALOG_DECODER_F_H
#define INCLUDED_SAT_OBSERVER_NOAA_ANALOG_DECODER_F_H

#include <sat_observer/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace sat_observer {

    /*!
     * \brief <+description of block+>
     * \ingroup sat_observer
     *
     */
    class SAT_OBSERVER_API noaa_analog_decoder_f : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<noaa_analog_decoder_f> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sat_observer::noaa_analog_decoder_f.
       *
       * To avoid accidental use of raw pointers, sat_observer::noaa_analog_decoder_f's
       * constructor is in a private implementation
       * class. sat_observer::noaa_analog_decoder_f::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string filename, std::string mode, int noaaNumber);
    };

  } // namespace sat_observer
} // namespace gr

#endif /* INCLUDED_SAT_OBSERVER_NOAA_ANALOG_DECODER_F_H */

