function(add_sep_library target_name)
    set(options)
    set(one_value_args)
    set(multi_value_args SOURCES CUDA_SOURCES DEPENDENCIES)
    cmake_parse_arguments(SEP_LIB "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    add_library(${target_name} STATIC ${SEP_LIB_SOURCES} ${SEP_LIB_CUDA_SOURCES})

    target_include_directories(${target_name}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_SOURCE_DIR}/src
    )

    if(NOT SEP_USE_CUDA)
        list(FILTER SEP_LIB_DEPENDENCIES EXCLUDE REGEX "^CUDA::")
    endif()
    if(SEP_LIB_DEPENDENCIES)
        target_link_libraries(${target_name}
            PUBLIC
                ${SEP_LIB_DEPENDENCIES}
        )
    endif()



    set_target_properties(${target_name} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    if(MSVC)
        target_compile_options(${target_name} PRIVATE /FI"${CMAKE_SOURCE_DIR}/src/engine/internal/glm_config.h")
    else()
        target_compile_options(${target_name} PRIVATE -include "${CMAKE_SOURCE_DIR}/src/engine/internal/glm_config.h")
    endif()

    if(SEP_USE_CUDA AND SEP_LIB_CUDA_SOURCES)
        set_target_properties(${target_name} PROPERTIES
            CUDA_SEPARABLE_COMPILATION ON
            CUDA_RESOLVE_DEVICE_SYMBOLS ON
            CUDA_STANDARD 17
        )
        if(CUDAToolkit_FOUND)
            target_link_libraries(${target_name} PUBLIC CUDA::toolkit)
            target_include_directories(${target_name} PUBLIC ${CUDAToolkit_INCLUDE_DIRS})
        endif()
    endif()
endfunction()