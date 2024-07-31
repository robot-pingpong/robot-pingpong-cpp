cmake_minimum_required(VERSION 3.22)
project(robot_pingpong_cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package(DynamixelSDK REQUIRED)
find_package(LinearMotor REQUIRED)

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
        src/visualizer.cpp
        src/visualizer.h
        src/timer.cpp
        src/timer.h
        src/vision.cpp
        src/vision.h
        src/arm.cpp
        src/arm.h
        src/servos/dynamixel.cpp
        src/servos/mx64_p2.h
        src/servos/protocol2.h
        src/servos/macros.h
        src/servos/dynamixel.h
        src/servos/mx28_p2.h
)

target_link_libraries(
        ${PROJECT_NAME}
        ${OpenCV_LIBS}
        dynamixel_sdk::dynamixel_sdk
        linear_motor::linear_motor
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
)