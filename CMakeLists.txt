cmake_minimum_required(VERSION 3.3)

# Guard against multiple includes
if(PRIMARY_INC)
	return()
else()
	set(PRIMARY_INC ON)
endif()

project(MediaCourier)
include(GNUInstallDirs)

# Global options
option(PREFIPV6 "Use IPv6 instead of IPv4" ON)

# Set global defaults
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -O0 -Wall -Werror")

if(PREFIPV6)
	add_definitions(-DUSE_IPV6)
endif()

add_subdirectory(Shared/)
add_subdirectory(MC-ChangelingShim/)
add_subdirectory(MC-TransmissionServer/)