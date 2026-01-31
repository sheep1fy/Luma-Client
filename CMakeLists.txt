cmake_minimum_required(VERSION 3.18)
project(LumaClient)

set(CMAKE_CXX_STANDARD 17)

# Linux desktop libraries
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Source files - Linux desktop only
set(SOURCES
    "src/luma_linux.c"
    "imgui/imgui.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
    "imgui/backends/imgui_impl_sdl3.cpp"  # SDL3 for Linux desktop
    "apis/bedrock_1_21_30.cpp"
)

# Create shared library
add_library(luma_client SHARED ${SOURCES})

# Linux desktop includes
target_include_directories(luma_client PRIVATE 
    .
    include
    src
    imgui
    imgui/backends
    ${GLEW_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

# Desktop OpenGL3 defines
target_compile_definitions(luma_client PRIVATE 
    IMGUI_IMPL_OPENGL_ES3=0
    IMGUI_IMPL_OPENGL_LOADER_GLEW=1
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
)

# Link Linux desktop libraries
target_link_libraries(luma_client 
    OpenGL::GL
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARIES}
    dl
    pthread
    rt
    m
)

# x86_64 optimizations
target_compile_options(luma_client PRIVATE 
    -O3 
    -march=native
    -flto
    -fvisibility=hidden
)

set_target_properties(luma_client PROPERTIES 
    POSITION_INDEPENDENT_CODE ON
)
