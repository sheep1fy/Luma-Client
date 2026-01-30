cmake_minimum_required(VERSION 3.16)
project(luma_client LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(luma_client SHARED
    src/luma_linux.cpp
    src/luma_core.cpp
    src/luma_module_manager.cpp
    src/luma_gui.cpp
    src/luma_hud_editor.cpp
    src/luma_game_api.cpp
    src/luma_config.cpp            # when you implement it
    src/modules/luma_fps_module.cpp
    src/modules/luma_cps_module.cpp
    src/modules/luma_keystrokes_module.cpp
    src/modules/luma_coords_module.cpp
    src/modules/luma_ping_module.cpp
    src/modules/luma_autosprint_module.cpp
    src/modules/luma_zoom_module.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_android.cpp
)

target_include_directories(luma_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

target_link_libraries(luma_client PRIVATE
    EGL
    GLESv2
    dl
    pthread
)
