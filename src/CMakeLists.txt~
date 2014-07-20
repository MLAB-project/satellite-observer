INCLUDE_DIRECTORIES(${sat-observer_SOURCE_DIR}/src/others)
INCLUDE_DIRECTORIES(${sat-observer_SOURCE_DIR}/src/orbitTools/core)
INCLUDE_DIRECTORIES(${sat-observer_SOURCE_DIR}/src/orbitTools/orbit)


ADD_EXECUTABLE(sat-observer main)

TARGET_LINK_LIBRARIES(sat-observer TLE SATELLITE CORE UTILITIES)


INSTALL(FILES ${sat-observer_SOURCE_DIR}/build/src/sat-observer DESTINATION ${sat-observer_SOURCE_DIR}/install)
