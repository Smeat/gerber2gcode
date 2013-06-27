cmake_minimum_required(VERSION 2.8)

project(gerber2gcode)



FILE(GLOB_RECURSE HDRS *.h)
 
#set(SRCS)

FILE(GLOB_RECURSE SRCS *.cpp)

file(GLOB_RECURSE to_remove CMakeCXXCompilerId.cpp)
if(to_remove)
	list(REMOVE_ITEM SRCS ${to_remove})
endif(to_remove)

SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic -ggdb -O0" ${CMAKE_CXX_FLAGS})



find_package(Boost COMPONENTS program_options REQUIRED)


include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(gerber2gcode ${HDRS} ${SRCS})

target_link_libraries(gerber2gcode ${Boost_LIBRARIES} )
