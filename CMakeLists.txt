cmake_minimum_required(VERSION 3.10)
project(luma_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# ======================================================================
# 1. DIRECTORY DEFINITIONS
# ======================================================================
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(APIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/apis)

# ======================================================================
# 2. INCLUDE DIRECTORIES
# ======================================================================
# This fixes the "imgui.h file not found" error
include_directories(${IMGUI_DIR})              # Core ImGui headers
include_directories(${IMGUI_BACKENDS_DIR})     # Backend implementations
include_directories(${SRC_DIR})                # Luma source files
include_directories(${INCLUDE_DIR})            # Public API headers
include_directories(${APIS_DIR})               # Bedrock API wrappers

# ======================================================================
# 3. IMGUI SOURCE FILES (Explicitly Listed)
# ======================================================================
# These must be compiled into the final .so library
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
    # Note: We do NOT include imgui_demo.cpp to reduce binary size
)

# ======================================================================
# 4. LUMA SOURCE FILES
# ======================================================================
set(LUMA_SOURCES
    # Core system files
    ${SRC_DIR}/luma_linux.c                    # Entry point + Dobby hooks
    ${SRC_DIR}/luma_module_manager.cpp         # UI and module registry
    ${SRC_DIR}/luma_core.cpp                   # Initialization logic
    ${SRC_DIR}/luma_config.cpp                 # Configuration management
    ${SRC_DIR}/luma_gui.cpp                    # GUI utilities
    ${SRC_DIR}/luma_hud_editor.cpp             # HUD customization
    ${SRC_DIR}/luma_game_api.cpp               # Game interaction layer
    ${SRC_DIR}/luma_camera_api.cpp             # Camera control

    # Bedrock API implementation
    ${APIS_DIR}/bedrock_1_21_30.cpp

    # Module implementations
    ${SRC_DIR}/modules/luma_autosprint_module.cpp
    ${SRC_DIR}/modules/luma_coords_module.cpp
    ${SRC_DIR}/modules/luma_cps_module.cpp
    ${SRC_DIR}/modules/luma_fps_module.cpp
    ${SRC_DIR}/modules/luma_keystrokes_module.cpp
    ${SRC_DIR}/modules/luma_ping_module.cpp
    ${SRC_DIR}/modules/luma_zoom_module.cpp
)

# ======================================================================
# 5. BUILD SHARED LIBRARY
# ======================================================================
add_library(luma_client SHARED 
    ${LUMA_SOURCES} 
    ${IMGUI_SOURCES}
)

# ======================================================================
# 6. LINK LIBRARIES
# ======================================================================
# GLESv3 - OpenGL ES 3.0 for rendering
# log     - Android/Linux logging (__android_log_print)
# dl      - Dynamic linking (dlsym, dlopen)
# dobby   - Hooking library (DobbyHook function)
target_link_libraries(luma_client 
    GLESv3 
    log
    dl
    dobby
)

# ======================================================================
# 7. COMPILER FLAGS
# ======================================================================
# -fvisibility=hidden: Hide internal symbols to reduce binary size
# -Os: Optimize for size (important for mod libraries)
# -ffunction-sections -fdata-sections: Allow linker to remove unused code
target_compile_options(luma_client PRIVATE
    -fvisibility=hidden
    -Os
    -ffunction-sections
    -fdata-sections
)

# ======================================================================
# 8. LINKER FLAGS
# ======================================================================
# --gc-sections: Remove unused sections
# -s: Strip debug symbols for smaller binary
target_link_options(luma_client PRIVATE
    -Wl,--gc-sections
    -s
)

# ======================================================================
# 9. OUTPUT CONFIGURATION
# ======================================================================
# Ensure the output is named libluma_client.so
set_target_properties(luma_client PROPERTIES
    PREFIX "lib"
    SUFFIX ".so"
    OUTPUT_NAME "luma_client"
)

# ======================================================================
# 10. INSTALL TARGET (Optional)
# ======================================================================
install(TARGETS luma_client
    LIBRARY DESTINATION ~/.local/share/mcpelauncher/mods/
)

message(STATUS "==================================================")
message(STATUS "Luma Client Build Configuration")
message(STATUS "==================================================")
message(STATUS "CMake Version:     ${CMAKE_VERSION}")
message(STATUS "C++ Standard:      ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "ImGui Directory:   ${IMGUI_DIR}")
message(STATUS "Output Binary:     libluma_client.so")
message(STATUS "==================================================")
