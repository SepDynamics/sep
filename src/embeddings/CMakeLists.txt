add_library(sep_embeddings STATIC
    simple_embedding_model.cpp
)



# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/embeddings/
        DESTINATION include/sep/embeddings
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/embeddings/
        DESTINATION include/sep/embeddings/internal
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")


target_include_directories(sep_embeddings
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

