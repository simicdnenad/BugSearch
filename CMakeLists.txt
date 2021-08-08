PROJECT(bugce)

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

add_definitions(-DLOGGER_AS_THREAD -DCHECK_TIME -DMULTI_THREAD)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
if (UNIX)
FILE(GLOB SRC_FILES "src/*Linux.cpp" "src/Logger_thread.cpp")
ENDIF()
if (WIN32)
FILE(GLOB SRC_FILES "src/*Win.cpp" "src/Logger_thread.cpp")
ENDIF()
#ADD_LIBRARY(BugceLib "src/Bug.cpp")
#SET_TARGET_PROPERTIES(BugceLib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY 
#                      ${PROJECT_SOURCE_DIR}/lib)

if (UNIX)
ADD_EXECUTABLE (BugceAppLinux ${SRC_FILES})
TARGET_LINK_LIBRARIES (BugceAppLinux LINK_PUBLIC pthread boost_program_options)
ENDIF()
if (WIN32)
	link_directories($ENV{BOOST_ROOT}/stage/lib)
	set(Boost_USE_STATIC_LIBS ON) 
	set(Boost_USE_MULTITHREADED ON)  
	set(Boost_USE_STATIC_RUNTIME OFF) 
	find_package(Boost
	1.65.1 REQUIRED							# Fail with error if Boost is not found
	COMPONENTS filesystem program_options	# Boost libraries by their canonical name
	)  
	if(Boost_FOUND)
		ADD_EXECUTABLE (BugceAppWin ${SRC_FILES})
		TARGET_LINK_LIBRARIES (BugceAppWin LINK_PUBLIC libboost_program_options-vc141-mt-gd-1_65_1)
	ENDIF()
ENDIF(WIN32)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/bin)




