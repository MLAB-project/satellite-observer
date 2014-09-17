#!/usr/bin/env python
#Satellite Observer. 
#The main objective of Satellite Observer is the autonomous capture 
#of signal from small satellites and the subsequent 
#decoding. It  keeps track of the trajectory, continuously updated of 
#Doppler shift and tunes the receiver.
#
#Copyright (C) 2014  Carlos Alberto Ruiz Naranjo
#carlosruiznaranjo@gmail.com

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU Affero General Public License as
#published by the Free Software Foundation, either version 3 of the
#License, or (at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Affero General Public License for more details.

#You should have received a copy of the GNU Affero General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import sat_observer
import wx

class top_block(gr.top_block):

    def __init__(self,f_in,f_out):
        gr.top_block.__init__(self)
        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 48000
        self.file_out = file_out = f_out
        self.file_in = file_in = f_in

        ##################################################
        # Blocks
        ##################################################
        self.sat_observer_noaa_analog_decoder_f_0 = sat_observer.noaa_analog_decoder_f(file_out, "ac", 15)
        self.rational_resampler_xxx_0_0 = filter.rational_resampler_fff(
                interpolation=441,
                decimation=192,
                taps=None,
                fractional_bw=None,
        )
        self.rational_resampler_xxx_0 = filter.rational_resampler_fff(
                interpolation=1,
                decimation=2,
                taps=None,
                fractional_bw=None,
        )
        self.freq_xlating_fir_filter_xxx_0 = filter.freq_xlating_fir_filter_ccc(1, (40, ), 10.966e3, samp_rate)
        self.blocks_wavfile_source_0 = blocks.wavfile_source(file_in, False)
        self.blocks_multiply_const_vxx_0_0 = blocks.multiply_const_vff((6.5, ))
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.analog_wfm_rcv_0 = analog.wfm_rcv(
        	quad_rate=samp_rate,
        	audio_decimation=5,
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.sat_observer_noaa_analog_decoder_f_0, 0))
        self.connect((self.blocks_wavfile_source_0, 1), (self.blocks_float_to_complex_0, 1))
        self.connect((self.blocks_wavfile_source_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.blocks_float_to_complex_0, 0), (self.freq_xlating_fir_filter_xxx_0, 0))
        self.connect((self.analog_wfm_rcv_0, 0), (self.rational_resampler_xxx_0_0, 0))
        self.connect((self.rational_resampler_xxx_0_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_multiply_const_vxx_0_0, 0))
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.analog_wfm_rcv_0, 0))


    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_file_out(self):
        return self.file_out

    def set_file_out(self, file_out):
        self.file_out = file_out

    def get_file_in(self):
        return self.file_in

    def set_file_in(self, file_in):
        self.file_in = file_in

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = top_block(sys.argv[1],sys.argv[2])
    tb.start(True)
    tb.wait()
    tb.stop()
