cmake_minimum_required(VERSION 3.5)
set(name Whiteboard)
project( ${name} )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ../include )
file(GLOB SOURCES "*.cpp")
add_executable( ${name} ${SOURCES})
target_link_libraries( ${name} ${OpenCV_LIBS} )
