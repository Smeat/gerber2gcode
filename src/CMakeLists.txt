project(gerber2gcode)



set(SRCS Util.cpp Cords.cpp GerberGeometry.cpp main.cpp Line.cpp GerberLoader.cpp GcodeGenerator.cpp)
set(HDRS Util.h Cords.h GerberGeometry.h Line.h Util.h GerberLoader.h GcodeGenerator.h Geometry.h)


SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic -ggdb -O0" ${CMAKE_CXX_FLAGS})

add_executable(gerber2gcode ${HDRS} ${SRCS})
