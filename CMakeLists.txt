cmake_minimum_required(VERSION 3.18)
project(LumaClient)

set(CMAKE_CXX_STANDARD 17)

# Android NDK paths
find_library(log-lib log)

# Source files
file(GLOB_RECURSE SOURCES 
    "src/*.c"
    "src/*.cpp"
    "imgui/*.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
    "imgui/backends/imgui_impl_android.cpp"
    "apis/*.cpp"
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

# Compiler flags
target_compile_definitions(luma_client PRIVATE 
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
    IMGUI_IMPL_OPENGL_ES3
)

# Link libraries
target_link_libraries(luma_client 
    GLESv3 
    log 
    android 
    ${log-lib}
)

# Optimization
set_target_properties(luma_client PROPERTIES 
    POSITION_INDEPENDENT_CODE ON
)
