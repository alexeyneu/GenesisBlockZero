 cmake_minimum_required(VERSION 3.18)

project(force VERSION 1.0)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

ADD_EXECUTABLE(force force.cpp)
target_link_libraries( force ${OpenCV_LIBS} )

