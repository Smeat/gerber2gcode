project(gerber2gcode)



FILE(GLOB_RECURSE HDRS *.h)
 
#set(SRCS)

FILE(GLOB_RECURSE SRCS *.cpp)

file(GLOB_RECURSE to_remove CMakeCXXCompilerId.cpp)
if(to_remove)
	list(REMOVE_ITEM SRCS ${to_remove})
endif(to_remove)

SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic -ggdb -O0" ${CMAKE_CXX_FLAGS})

add_executable(gerber2gcode ${HDRS} ${SRCS})
