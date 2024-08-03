cmake_minimum_required(VERSION 3.22)
project(robot_pingpong_cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package(DynamixelSDK REQUIRED)
find_package(LinearMotor REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
        src/vision/capture.cpp
        src/vision/capture.h
        src/constants.h
        src/vision/tracker.cpp
        src/vision/tracker.h
        src/vision/dlt.cpp
        src/vision/dlt.h
        src/control/linear_motor.cpp
        src/control/linear_motor.h
        src/vision/visualizer.cpp
        src/vision/visualizer.h
        src/utils/timer.cpp
        src/utils/timer.h
        src/vision/vision.cpp
        src/vision/vision.h
        src/control/arm.cpp
        src/control/arm.h
        src/dynamixel/dynamixel.cpp
        src/dynamixel/mx64_p2.h
        src/dynamixel/protocol2.h
        src/dynamixel/macros.h
        src/dynamixel/dynamixel.h
        src/dynamixel/mx28_p2.h
        src/predictor.cpp
        src/predictor.h
        src/dynamixel/base_motor.h
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