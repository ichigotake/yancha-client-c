cmake_minimum_required(VERSION 2.8.4)
project(core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FindCURL)
include(FindJansson)
include_directories(
    ${JANSSON_INCLUDE_DIRS}
    deps/picotest
)

set(UNIT_TEST_SOURCE_FILES
    deps/picotest/picotest.c
    yancha.c
    test.c
)

add_executable(core main.c)
add_library(libtest ${UNIT_TEST_SOURCE_FILES})
target_link_libraries(core libtest ${CURL_LIBRARIES} ${JANSSON_LIBRARIES})
