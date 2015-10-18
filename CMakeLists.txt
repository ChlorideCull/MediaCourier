cmake_minimum_required(VERSION 3.3)
project(MediaCourier)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp StreamProcess.cpp StreamProcess.hpp Util.cpp Util.hpp)
add_definitions(-DBINDIR=\"${CMAKE_CURRENT_BINARY_DIR}\" -DDEBUG)
add_executable(MediaCourier ${SOURCE_FILES})
target_link_libraries(MediaCourier -lpthread)
add_library(MCNetRedir SHARED socketredirect.c)