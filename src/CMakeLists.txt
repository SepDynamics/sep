# Gather all workbench sources and demos
file(GLOB_RECURSE WB_SOURCES
    "api_main.cpp"
)    
# Add component subdirectories - only core components needed for quant processing
add_subdirectory(glad)
add_subdirectory(engine)
# add_subdirectory(embeddings)
add_subdirectory(memory)
add_subdirectory(quantum)
add_subdirectory(connectors)



