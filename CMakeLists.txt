cmake_minimum_required(VERSION 3.18)
project(LumaClient)

set(CMAKE_CXX_STANDARD 17)

# Android NDK paths
find_library(log-lib log)

# Source files - ONLY Android/GLES3 backends
set(SOURCES
    "src/luma_linux.c"
    "imgui/imgui.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
    "imgui/backends/imgui_impl_android.cpp"
    "apis/bedrock_1_21_30.cpp"
)

# Create shared library
add_library(luma_client SHARED ${SOURCES})

# Include directories
target_include_directories(luma_client PRIVATE 
    .
    include
    src
    imgui
    imgui/backends
)

# Compiler flags - Android/GLES3 only
target_compile_definitions(luma_client PRIVATE 
    IMGUI_IMPL_OPENGL_ES3=1
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM=1
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
)

# Link ONLY Android/GLESv3 libraries
target_link_libraries(luma_client 
    GLESv3 
    EGL
    log 
    android
)

# Optimization flags
target_compile_options(luma_client PRIVATE 
    -O3 
    -fvisibility=hidden
    -ffunction-sections
    -fdata-sections
)

set_target_properties(luma_client PROPERTIES 
    POSITION_INDEPENDENT_CODE ON
)
