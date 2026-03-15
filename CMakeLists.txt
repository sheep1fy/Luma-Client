cmake_minimum_required(VERSION 3.10)
project(luma_client VERSION 2.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── Build type ───────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS_RELEASE   "-O3 -march=native -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# ─── Include directories ───────────────────────────────────────
include_directories(
    ${CMAKE_SOURCE_DIR}/include        # Public headers (highest priority)
    ${CMAKE_SOURCE_DIR}/imgui          # ImGui root
    ${CMAKE_SOURCE_DIR}/imgui/backends # ImGui backends
)

# ─── ImGui sources ────────────────────────────────────────────
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# ─── Module sources ───────────────────────────────────────────
set(MODULE_SOURCES
    src/modules/luma_fps_module.cpp
    src/modules/luma_coords_module.cpp
    src/modules/luma_cps_module.cpp
    src/modules/luma_keystrokes_module.cpp
    src/modules/luma_ping_module.cpp
    src/modules/luma_zoom_module.cpp
    src/modules/luma_autosprint_module.cpp
)

# ─── Core sources ─────────────────────────────────────────────
set(CORE_SOURCES
    src/luma_linux.c
    src/luma_core.cpp
    src/luma_gui.cpp
    src/luma_module_manager.cpp
    src/luma_hud_editor.cpp
    src/luma_camera_api.cpp
    src/luma_game_api.cpp
    src/luma_config.cpp
    ${MODULE_SOURCES}
    ${IMGUI_SOURCES}
)

# ─── Shared library target ────────────────────────────────────
add_library(luma_client SHARED ${CORE_SOURCES})

# ─── Compile definitions ──────────────────────────────────────
target_compile_definitions(luma_client PRIVATE
    IMGUI_IMPL_OPENGL_ES2          # OpenGL ES 2 backend
    LUMA_VERSION="v2.0.0"
    LUMA_TARGET_MINECRAFT="1.21.60"
)

# ─── Link libraries ───────────────────────────────────────────
target_link_libraries(luma_client
    GLESv2
    dl           # dlsym / RTLD_NEXT
)

# X11 is optional (used for diagnostics only; SDL2 hooking is primary)
if(NOT ANDROID)
    find_package(X11)
    if(X11_FOUND)
        target_link_libraries(luma_client ${X11_LIBRARIES})
        target_compile_definitions(luma_client PRIVATE HAVE_X11)
    endif()
endif()

# ─── Output directory ─────────────────────────────────────────
set_target_properties(luma_client PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    PREFIX ""                          # libluma_client.so, not libluma_client.so
)

# ─── Install target ───────────────────────────────────────────
set(MCPE_MOD_DIR "$ENV{HOME}/.local/share/mcpelauncher/mods/Luma Client/v2.0.0/x86_64")

add_custom_target(install_mod
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MCPE_MOD_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_BINARY_DIR}/libluma_client.so"
        "${MCPE_MOD_DIR}/libluma_client.so"
    COMMAND chmod +x "${MCPE_MOD_DIR}/libluma_client.so"
    DEPENDS luma_client
    COMMENT "Installing Luma Client to mcpelauncher mods directory"
)

message(STATUS "Luma Client v2.0.0 - Configuration complete")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install path: ${MCPE_MOD_DIR}")
