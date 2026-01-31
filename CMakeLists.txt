cmake_minimum_required(VERSION 3.18.1)
project(LumaClient VERSION 1.0.1 LANGUAGES C CXX)

# Android detection
set(ANDROID_BUILD ON)

# Global flags
add_compile_definitions(
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
    IMGUI_IMPL_OPENGL_ES3=0
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM=1
)

# ImGui static library
file(GLOB IMGUI_SOURCES 
    imgui/*.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC imgui imgui/backends)

# Main Luma Client shared library (.so)
add_library(luma_client SHARED 
    src/luma_linux.c
)

target_include_directories(luma_client PRIVATE
    .
    include
    src
    imgui
)

target_link_libraries(luma_client 
    imgui
    GLESv3
    log    # Android logging
    dl     # dlsym for hooking
)

# Android optimizations
if(ANDROID)
    target_compile_options(luma_client PRIVATE
        -O3
        -march=x86-64
        -flto
        -fvisibility=hidden
        -fPIC
    )
endif()

set_target_properties(luma_client PROPERTIES
    OUTPUT_NAME "luma_client"
    POSITION_INDEPENDENT_CODE ON
)
