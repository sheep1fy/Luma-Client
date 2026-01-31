cmake_minimum_required(VERSION 3.20)
project(Luma-Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Optimization flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DNDEBUG")

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLEW REQUIRED glew)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/imgui/backends
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/apis/bedrock_1_21_30.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# Create shared library
add_library(luma_client SHARED ${SOURCES})

# Fix ImGui OpenGL3 loader issue - explicitly include backends folder first
target_include_directories(luma_client PRIVATE 
    ${CMAKE_SOURCE_DIR}/imgui/backends  # CRITICAL: imgui_impl_opengl3_loader.h lives here
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Define ImGui OpenGL loader (choose one)
target_compile_definitions(luma_client PRIVATE 
    IMGUI_IMPL_OPENGL_LOADER_GLAD
    # Alternative: IMGUI_IMPL_OPENGL_LOADER_GLEW  (if using GLEW)
)

# Link libraries
target_link_libraries(luma_client PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
    -fvisibility=hidden
)

# Set visibility for shared library
set_target_properties(luma_client PROPERTIES 
    CXX_VISIBILITY_PRESET hidden
)

# Install targets
install(TARGETS luma_client DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
