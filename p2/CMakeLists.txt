cmake_minimum_required(VERSION 3.6)
project(p2)

exec_program(pkg-config ARGS --cflags --libs opencv
        OUTPUT_VARIABLE OPENCV_FLAGS)
#set(OPENCV_FLAGS "${OPENCV_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall ${OPENCV_FLAGS}")

set(SOURCE_FILES vseg.cpp)
add_executable(p2 ${SOURCE_FILES})