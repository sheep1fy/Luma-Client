# Update the minimum version to stop the warnings
cmake_minimum_required(VERSION 3.10)

project(luma_client)

# Include directories (ensure these match your folder structure)
include_directories(src)
include_directories(include)

# Define the source files you actually have
set(SOURCE_FILES
    src/luma_linux.c
    src/luma_module_manager.cpp
)

# Tell CMake to build the library using YOUR files
add_library(luma_client SHARED ${SOURCE_FILES})

# Link necessary libraries (GLESv3 for rendering, Log for Android logging)
target_link_libraries(luma_client GLESv3 log)
