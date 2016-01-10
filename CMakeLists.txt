cmake_minimum_required(VERSION 3.3)

# Guard against multiple includes
if(PRIMARY_INC)
	return()
else()
	set(PRIMARY_INC ON)
endif()

project(MediaCourier)

# Global options
option(PREFIPV6 "Use IPv6 instead of IPv4" ON)

# Set global defaults
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if(PREFIPV6)
	add_definitions(-DUSE_IPV6)
endif()
if(CMAKE_BUILD_TYPE EQUAL Debug)
	add_definitions(-DDEBUG)
	add_compile_options(-O0 -Wall -Werror)
endif()

add_subdirectory(Shared/)
add_subdirectory(MC-TransmissionServer/)

