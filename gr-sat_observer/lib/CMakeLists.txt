# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS} /home/carlos/Escritorio/gr-sat_observer/lib/atpdec /usr/local/lib)

list(APPEND sat_observer_sources
    noaa_analog_decoder_f_impl.cc
    decode.cpp
    coef.cpp
    dsp.cpp
    fcolor.cpp
    filter.cpp
    image.cpp
    reg.cpp
)

set(sat_observer_sources "${sat_observer_sources}" PARENT_SCOPE)
if(NOT sat_observer_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT sat_observer_sources)

add_library(gnuradio-sat_observer SHARED ${sat_observer_sources})
target_link_libraries(gnuradio-sat_observer ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} -lm -lsndfile -lpng)
set_target_properties(gnuradio-sat_observer PROPERTIES DEFINE_SYMBOL "gnuradio_sat_observer_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-sat_observer PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-sat_observer
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)


########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_sat_observer_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_sat_observer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_sat_observer.cc
)

add_executable(test-sat_observer ${test_sat_observer_sources})

target_link_libraries(
  test-sat_observer
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}  
  gnuradio-sat_observer
)

GR_ADD_TEST(test_sat_observer test-sat_observer)
