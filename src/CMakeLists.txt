project(gerber2gcode)

set(SRCS Cords.cpp GerberGeometry.cpp main.cpp Line.cpp GerberLoad.cpp)
set(HDRS Cords.h GerberGeometry.h Line.h Util.h GerberLoad.h)

SET(CMAKE_CXX_FLAGS "-std=c++0x" ${CMAKE_CXX_FLAGS})

add_executable(gerber2gcode ${HDRS} ${SRCS})
