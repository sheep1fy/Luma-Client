cmake_minimum_required(VERSION 3.19)
project(luma_client)

set(CMAKE_CXX_STANDARD 17)

# 1. Define Paths
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(APIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/apis)

# 2. Include Directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${SRC_DIR})
include_directories(${SRC_DIR}/modules)
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)
include_directories(${APIS_DIR})

# 3. Collect ImGui Sources (imgui_demo.cpp excluded - not needed for runtime)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# 4. Define Luma Sources
set(LUMA_SOURCES
    ${SRC_DIR}/luma_linux.c
    ${SRC_DIR}/luma_module_manager.cpp
    ${SRC_DIR}/luma_core.cpp
    ${SRC_DIR}/luma_gui.cpp
    ${SRC_DIR}/luma_hud_editor.cpp
    ${SRC_DIR}/luma_camera_api.cpp
    ${SRC_DIR}/luma_game_api.cpp
    ${SRC_DIR}/luma_config.cpp
    ${SRC_DIR}/modules/luma_fps_module.cpp
    ${SRC_DIR}/modules/luma_cps_module.cpp
    ${SRC_DIR}/modules/luma_keystrokes_module.cpp
    ${SRC_DIR}/modules/luma_coords_module.cpp
    ${SRC_DIR}/modules/luma_ping_module.cpp
    ${SRC_DIR}/modules/luma_zoom_module.cpp
    ${SRC_DIR}/modules/luma_autosprint_module.cpp
    ${APIS_DIR}/bedrock_1_21_30.cpp
)

# 5. Build Shared Library
add_library(luma_client SHARED ${LUMA_SOURCES} ${IMGUI_SOURCES})

# 6. Define ImGui Math Operators (fixes ImVec2 addition error)
target_compile_definitions(luma_client PRIVATE IMGUI_DEFINE_MATH_OPERATORS)

# 7. Link Libraries
# FIXED: Changed from GLESv3 to GLESv2 (available on all Mesa systems)
# OpenGL ES 2.0 is backward compatible and widely supported on Linux
target_link_libraries(luma_client GLESv2 log)
