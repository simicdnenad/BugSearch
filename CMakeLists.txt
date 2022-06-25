PROJECT(BugSearch)

CMAKE_MINIMUM_REQUIRED(VERSION 3.19.1)
CMAKE_POLICY(SET CMP0015 NEW)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Debug)
if (UNIX)
SET(CMAKE_CXX_FLAGS "-std=c++11 -fPIC")
SET(CMAKE_CXX_FLAGS "-msse3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
SET(CMAKE_C_FLAGS "-msse3")
SET(CMAKE_C_FLAGS "-mavx")
endif(UNIX)

IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

add_subdirectory(src)
add_subdirectory(tst)
add_subdirectory(lib/googletest)



