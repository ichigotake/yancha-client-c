cmake_minimum_required(VERSION 2.8.4)
project(yancha_client_c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
add_executable(yancha_client_c ${SOURCE_FILES})