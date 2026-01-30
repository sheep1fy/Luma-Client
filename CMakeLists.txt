cmake_minimum_required(VERSION 3.16)
project(luma_client LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Luma Client shared library
add_library(luma_client SHARED

    # Core Luma files
    src/luma_linux.cpp
    src/luma_core.cpp
    src/luma_module_manager.cpp
    src/luma_gui.cpp
    src/luma_hud_editor.cpp
    src/luma_game_api.cpp
    src/luma_camera_api.cpp
    src/luma_config.cpp
    
    # All 7 modules
    src/modules/luma_fps_module.cpp
    src/modules/luma_cps_module.cpp
    src/modules/luma_keystrokes_module.cpp
    src/modules/luma_coords_module.cpp
    src/modules/luma_ping_module.cpp
    src/modules/luma_autosprint_module.cpp
    src/modules/luma_zoom_module.cpp
    
    # Bedrock 1.21.30 API
    apis/bedrock_1_21_30.cpp
    
    # Dear ImGui
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# Include directories
target_include_directories(luma_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/apis
)

# Link libraries (MCPelauncher + Linux)
target_link_libraries(luma_client PRIVATE
    EGL
    dl
    pthread 
    log
)

target_link_options(luma_client PRIVATE
    -static-libstdc++
    -static-libgcc
    )
# Compiler optimizations
target_compile_definitions(luma_client PRIVATE
    IMGUI_IMPL_OPENGL_ES2
    IMGUI_DEFINE_MATH_OPERATORS
)

# Release build optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(luma_client PRIVATE -O3 -DNDEBUG)
endif()

# Strip symbols for smaller .so and remove 'lib' prefix
set_target_properties(luma_client PROPERTIES
    OUTPUT_NAME "luma_client"
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Print success message
message(STATUS "Luma Client will build as: ${CMAKE_BINARY_DIR}/luma_client.so")
