cmake_minimum_required(VERSION 3.28)
project(robot_pingpong_cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(robot_pingpong_cpp src/main.cpp
        src/capture.cpp
        src/capture.h
        src/constants.h)

target_link_libraries(robot_pingpong_cpp ${OpenCV_LIBS})