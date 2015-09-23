cmake_minimum_required(VERSION 3.3)
project(MediaCourier)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(MediaCourier ${SOURCE_FILES})
add_library(MCNetRedir SHARED socketredirect.c)