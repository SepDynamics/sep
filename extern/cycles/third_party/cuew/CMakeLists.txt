set(SRC
	src/cuew.c
)

set(SRC_HEADERS
	include/cuew.h
)

include_directories(
	.
	include
)

add_library(extern_cuew ${SRC} ${SRC_HEADERS})
set_target_properties(extern_cuew PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)
