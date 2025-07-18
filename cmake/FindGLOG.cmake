# Find GLOG library
#
# This module defines:
#  GLOG_FOUND - Whether glog was found
#  GLOG_INCLUDE_DIRS - The glog include directories
#  GLOG_LIBRARIES - The glog libraries

include(FindPackageHandleStandardArgs)

# Try to find the header
find_path(GLOG_INCLUDE_DIR
    NAMES glog/logging.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
)

# Try to find the library
find_library(GLOG_LIBRARY
    NAMES glog
    PATHS
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        /opt/local/lib
)

# Handle the QUIETLY and REQUIRED arguments
find_package_handle_standard_args(GLOG
    REQUIRED_VARS 
        GLOG_LIBRARY
        GLOG_INCLUDE_DIR
)

if(GLOG_FOUND)
    set(GLOG_LIBRARIES ${GLOG_LIBRARY})
    set(GLOG_INCLUDE_DIRS ${GLOG_INCLUDE_DIR})
    
    if(NOT TARGET GLOG::GLOG)
        add_library(GLOG::GLOG UNKNOWN IMPORTED)
        set_target_properties(GLOG::GLOG PROPERTIES
            IMPORTED_LOCATION "${GLOG_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GLOG_INCLUDE_DIR}"
        )
    endif()
endif()

mark_as_advanced(
    GLOG_INCLUDE_DIR
    GLOG_LIBRARY
)