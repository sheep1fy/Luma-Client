cmake_minimum_required(VERSION 3.16)
project(flarial_linux LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(flarial_linux SHARED
    src/flarial_linux.cpp
    src/flarial_core.cpp
    src/module_manager.cpp
    src/gui.cpp
    src/hud_editor.cpp
    src/game_api.cpp
    src/config.cpp
    src/modules/fps_module.cpp
    src/modules/cps_module.cpp
    src/modules/keystrokes_module.cpp
    src/modules/coords_module.cpp
    src/modules/ping_module.cpp
    src/modules/autosprint_module.cpp
    src/modules/zoom_module.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_android.cpp
)

target_include_directories(flarial_linux PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

target_link_libraries(flarial_linux PRIVATE
    EGL
    GLESv2
    dl
    pthread
)
