INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_SAT_OBSERVER sat_observer)

FIND_PATH(
    SAT_OBSERVER_INCLUDE_DIRS
    NAMES sat_observer/api.h
    HINTS $ENV{SAT_OBSERVER_DIR}/include
        ${PC_SAT_OBSERVER_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    SAT_OBSERVER_LIBRARIES
    NAMES gnuradio-sat_observer
    HINTS $ENV{SAT_OBSERVER_DIR}/lib
        ${PC_SAT_OBSERVER_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SAT_OBSERVER DEFAULT_MSG SAT_OBSERVER_LIBRARIES SAT_OBSERVER_INCLUDE_DIRS)
MARK_AS_ADVANCED(SAT_OBSERVER_LIBRARIES SAT_OBSERVER_INCLUDE_DIRS)

