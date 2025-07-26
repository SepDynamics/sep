set(IMPLOT_SRC
    implot.cpp
    implot_items.cpp
    implot_demo.cpp
)

add_library(implot STATIC ${IMPLOT_SRC})

# include directories
target_include_directories(implot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
)

target_link_libraries(implot PUBLIC imgui)
