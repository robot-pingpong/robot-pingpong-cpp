find_path(DynamixelSDK_INCLUDE_DIR dynamixel_sdk.h
        PATHS
        ENV DYNAMIXEL_SDK_DIR
        ${DynamixelSDK_DIR}
        PATH_SUFFIXES
        include)

find_library(DynamixelSDK_LIBRARY
        NAMES dxl_x64_cpp
        PATHS
        ENV DYNAMIXEL_SDK_DIR
        ${DynamixelSDK_DIR}
        PATH_SUFFIXES
        lib)

mark_as_advanced(DynamixelSDK_INCLUDE_DIR DynamixelSDK_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DynamixelSDK REQUIRED_VARS DynamixelSDK_INCLUDE_DIR DynamixelSDK_LIBRARY)

if (DynamixelSDK_FOUND AND NOT TARGET dynamixel_sdk::dynamixel_sdk)
    add_library(dynamixel_sdk::dynamixel_sdk UNKNOWN IMPORTED)
    set_target_properties(dynamixel_sdk::dynamixel_sdk PROPERTIES
            IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
            IMPORTED_LOCATION "${DynamixelSDK_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${DynamixelSDK_INCLUDE_DIR}"
    )
endif ()
