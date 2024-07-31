find_path(LinearMotor_INCLUDE_DIR AXL.h
        PATHS
        ENV LINEAR_MOTOR_DIR
        ${LinearMotor_DIR}
        PATH_SUFFIXES
        include)

find_library(LinearMotor_LIBRARY
        NAMES AXL
        PATHS
        ENV LINEAR_MOTOR_DIR
        ${LinearMotor_DIR}
        PATH_SUFFIXES
        lib)

mark_as_advanced(LinearMotor_INCLUDE_DIR LinearMotor_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LinearMotor REQUIRED_VARS LinearMotor_INCLUDE_DIR LinearMotor_LIBRARY)

if (LinearMotor_FOUND AND NOT TARGET linear_motor::linear_motor)
    add_library(linear_motor::linear_motor UNKNOWN IMPORTED)
    set_target_properties(linear_motor::linear_motor PROPERTIES
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            IMPORTED_LOCATION "${LinearMotor_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${LinearMotor_INCLUDE_DIR}"
    )
endif ()
