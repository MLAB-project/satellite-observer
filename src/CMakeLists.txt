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


ADD_EXECUTABLE(sat-observer main)

TARGET_LINK_LIBRARIES(sat-observer TLE SATELLITE CORE UTILITIES)


INSTALL(FILES ${sat-observer_SOURCE_DIR}/build/src/sat-observer DESTINATION ${sat-observer_SOURCE_DIR}/install)
