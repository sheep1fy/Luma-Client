cmake_minimum_required(VERSION 3.18)
project(LumaClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Linux desktop libraries (no pkg-config needed)
find_package(OpenGL REQUIRED)

# Source files - ONLY files that exist
set(SOURCES
    src/luma_linux.c
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_opengl3_loader.h  # GLEW loader
)

# Optional files (only if they exist)
list(APPEND SOURCES apis/bedrock_1_21_30.cpp)

# Create shared library
add_library(luma_client SHARED ${SOURCES})

# Include directories
target_include_directories(luma_client PRIVATE 
    .
    include/
    src/
    imgui/
    imgui/backends/
)

# Desktop OpenGL3 + GLEW defines
target_compile_definitions(luma_client PRIVATE 
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM=1
    IMGUI_IMPL_OPENGL_ES3=0
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
)

# Link Linux desktop libraries
target_link_libraries(luma_client 
    GL 
    GLEW 
    dl 
    pthread
)

# x86_64 optimizations
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    target_compile_options(luma_client PRIVATE 
        -O3 
        -march=native 
        -flto 
        -fvisibility=hidden
    )
endif()
