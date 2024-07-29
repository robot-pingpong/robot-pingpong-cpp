
if (WIN32 OR CYGWIN)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
else ()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif ()

if (NOT LinearMotor_DIR)
    set(LinearMotor_DIR $ENV{LinearMotor_DIR})
endif ()

if (LinearMotor_DIR)
    set(LinearMotor_FIND_QUIETLY TRUE)
endif ()

find_path(LinearMotor_INCLUDE_DIR AXL.h
        PATHS ${LinearMotor_DIR}/include
        NO_DEFAULT_PATH)

find_library(LinearMotor_LIBRARY
        NAMES AXL
        PATHS ${LinearMotor_DIR}/lib
        NO_DEFAULT_PATH)

if (LinearMotor_INCLUDE_DIR AND LinearMotor_LIBRARY)
    set(LinearMotor_FOUND TRUE)
    set(LinearMotor_INCLUDE_DIRS ${LinearMotor_INCLUDE_DIR})
    set(LinearMotor_LIBRARIES ${LinearMotor_LIBRARY})
else ()
    set(LinearMotor_FOUND FALSE)
    set(LinearMotor_INCLUDE_DIRS)
    set(LinearMotor_LIBRARIES)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LinearMotor DEFAULT_MSG LinearMotor_INCLUDE_DIR LinearMotor_LIBRARY)

mark_as_advanced(LinearMotor_INCLUDE_DIR LinearMotor_LIBRARY)
