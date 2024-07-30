cmake_minimum_required(VERSION 3.28)
project(robot_pingpong_cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package(LinearMotor REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${LinearMotor_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp
        src/capture.cpp
        src/capture.h
        src/constants.h
        src/tracker.cpp
        src/tracker.h
        src/dlt.cpp
        src/dlt.h
        src/linear_motor.cpp
        src/linear_motor.h
)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${LinearMotor_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
)