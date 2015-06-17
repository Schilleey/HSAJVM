cmake_minimum_required(VERSION 2.8)

project(hsajvm)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

add_executable(hsajvm ${SOURCE_FILES})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
