add_library(sep_embeddings STATIC
    simple_embedding_model.cpp
)

target_include_directories(sep_embeddings PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

