cmake_minimum_required(VERSION 2.8)

project(gerber2gcode)

set(HDRS ./Cords.h
./GerberLoader.h
./Geometry.h
./image/SVGExport.h
./shapes/Circle.h
./shapes/Shape.h
./shapes/Rectangle.h
./shapes/Line.h
./FileLoader.h
./Util.h
./GcodeGenerator.h
./GerberGeometry.h
)
 
set(SRCS ./main.cpp
./GcodeGenerator.cpp
./GerberLoader.cpp
./Util.cpp
./Cords.cpp
./image/SVGExport.cpp
./shapes/Circle.cpp
./shapes/Line.cpp
./shapes/Rectangle.cpp
./GerberGeometry.cpp
)


message(STATUS ${HDRS})

file(GLOB_RECURSE to_remove CMakeCXXCompilerId.cpp)
if(to_remove)
	list(REMOVE_ITEM SRCS ${to_remove})
endif(to_remove)

SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic -ggdb -O0" ${CMAKE_CXX_FLAGS})



find_package(Boost COMPONENTS program_options REQUIRED)


include_directories(${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(gerber2gcode ${HDRS} ${SRCS})

target_link_libraries(gerber2gcode ${Boost_LIBRARIES} )
