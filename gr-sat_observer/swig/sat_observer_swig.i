/* -*- c++ -*- */

#define SAT_OBSERVER_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "sat_observer_swig_doc.i"

%{
#include "sat_observer/noaa_analog_decoder_f.h"
%}


%include "sat_observer/noaa_analog_decoder_f.h"
GR_SWIG_BLOCK_MAGIC2(sat_observer, noaa_analog_decoder_f);
