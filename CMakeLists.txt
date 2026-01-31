cmake_minimum_required(VERSION 3.18.1)
project(LumaClient VERSION 1.0.1 LANGUAGES C CXX)

# Global ImGui flags
add_compile_definitions(
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
    IMGUI_IMPL_OPENGL_ES3=0
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM=1
    luma_client_EXPORTS
)

# ImGui static library (CORRECTED - only specific files)
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC imgui imgui/backends)

# Main Luma Client shared library - ONLY luma_linux.c + ImGui
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
    log
    dl
    atomic  # ← CRITICAL: -latomic was in your build log
    m       # ← CRITICAL: -lm was in your build log
)

# Android x86_64 optimizations (EXACTLY matching your build log)
if(ANDROID)
    target_compile_options(luma_client PRIVATE
        -g
        -DANDROID
        -fdata-sections
        -ffunction-sections
        -funwind-tables
        -fstack-protector-strong
        -no-canonical-prefixes
        -D_FORTIFY_SOURCE=2
        -Wformat
        -Werror=format-security
        -O3
        -DNDEBUG
        -fPIC
        -march=x86-64
        -flto
        -fvisibility=hidden
    )
endif()

set_target_properties(luma_client PROPERTIES
    OUTPUT_NAME "luma_client"
    POSITION_INDEPENDENT_CODE ON
)
