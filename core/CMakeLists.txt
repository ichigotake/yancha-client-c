cmake_minimum_required(VERSION 2.8.4)
project(core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(FindCURL)
include_directories(
    deps/picotest
)

set(UNIT_TEST_SOURCE_FILES
    deps/picotest/picotest.c
    yancha.c
    test.c
)

add_executable(core main.c)
add_library(libtest ${UNIT_TEST_SOURCE_FILES})
target_link_libraries(core libtest ${CURL_LIBRARIES})
