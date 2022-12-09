set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

if (UNIX)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fprofile-arcs -ftest-coverage")
ENDIF()

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

if (UNIX)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}Linux_lib gtest)
ENDIF()
if (WIN32)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}Win_lib gtest)
ENDIF()
