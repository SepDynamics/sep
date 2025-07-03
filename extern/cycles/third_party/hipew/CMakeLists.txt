set(INC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(INC_SYS)

set(SRC
  src/hipew.c
  include/hipew.h
)

set(LIB)

if(HIPRT_INCLUDE_DIR)
  list(APPEND INC_SYS
    ${HIPRT_INCLUDE_DIR}
  )

  list(APPEND SRC
    src/hiprtew.cc
    include/hiprtew.h
  )
  enable_language(CXX)
endif()

add_library(extern_hipew ${SRC})
target_include_directories(extern_hipew PUBLIC ${INC} ${INC_SYS})
set_target_properties(extern_hipew PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)
