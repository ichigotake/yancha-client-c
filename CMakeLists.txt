cmake_minimum_required(VERSION 2.8.4)
project(yancha_client_c)

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

add_executable(yancha_client_c main.c)
add_library(libtest ${UNIT_TEST_SOURCE_FILES})
target_link_libraries(yancha_client_c libtest ${CURL_LIBRARIES})
