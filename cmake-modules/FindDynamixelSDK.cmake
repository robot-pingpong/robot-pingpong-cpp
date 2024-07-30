if (NOT DynamixelSDK_DIR)
    set(DynamixelSDK_DIR $ENV{DynamixelSDK_DIR})
endif ()

if (DynamixelSDK_DIR)
    set(DynamixelSDK_FIND_QUIETLY TRUE)
endif ()

find_path(DynamixelSDK_INCLUDE_DIR dynamixel_sdk.h
        PATHS ${DynamixelSDK_DIR}/include
        NO_DEFAULT_PATH)

find_library(DynamixelSDK_LIBRARY
        NAMES dxl_x64_cpp
        PATHS ${DynamixelSDK_DIR}/lib
        NO_DEFAULT_PATH)

if (DynamixelSDK_INCLUDE_DIR AND DynamixelSDK_LIBRARY)
    set(DynamixelSDK_FOUND TRUE)
    set(DynamixelSDK_INCLUDE_DIRS ${DynamixelSDK_INCLUDE_DIR})
    set(DynamixelSDK_LIBRARIES ${DynamixelSDK_LIBRARY})
else ()
    set(DynamixelSDK_FOUND FALSE)
    set(DynamixelSDK_INCLUDE_DIRS)
    set(DynamixelSDK_LIBRARIES)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DynamixelSDK DEFAULT_MSG DynamixelSDK_INCLUDE_DIR DynamixelSDK_LIBRARY)

mark_as_advanced(DynamixelSDK_INCLUDE_DIR DynamixelSDK_LIBRARY)
