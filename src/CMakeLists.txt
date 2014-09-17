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

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


INCLUDE_DIRECTORIES(${sat-observer_SOURCE_DIR}/src/others)
INCLUDE_DIRECTORIES(${sat-observer_SOURCE_DIR}/src/orbitTools/core)
INCLUDE_DIRECTORIES(${sat-observer_SOURCE_DIR}/src/orbitTools/orbit)
INCLUDE_DIRECTORIES(${sat-observer_SOURCE_DIR}/src/socket)



ADD_EXECUTABLE(sat-observer sat-observer_main)
ADD_EXECUTABLE(client client_main)
ADD_EXECUTABLE(server server_main)


TARGET_LINK_LIBRARIES(sat-observer TLE SATELLITE CORE UTILITIES)
TARGET_LINK_LIBRARIES(client CLIENT_S SOCKET UTILITIES pthread boost_system)
TARGET_LINK_LIBRARIES(server SERVER_S SOCKET UTILITIES)


INSTALL(FILES ${sat-observer_SOURCE_DIR}/build/src/sat-observer DESTINATION ${sat-observer_SOURCE_DIR}/install)
INSTALL(FILES ${sat-observer_SOURCE_DIR}/build/src/client DESTINATION ${sat-observer_SOURCE_DIR}/install)
INSTALL(FILES ${sat-observer_SOURCE_DIR}/build/src/server DESTINATION ${sat-observer_SOURCE_DIR}/install)




