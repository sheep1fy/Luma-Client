cmake_minimum_required(VERSION 3.10)
project(luma_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# ======================================================================
# 0. FIX: Disable ARM64 for x86_64 Linux builds (CI/CD fix)
# ======================================================================
if(CMAKE_SYSTEM_PROCESSOR MATCHES "amd64|x86_64" AND NOT ANDROID)
    message(STATUS "Detected x86_64 Linux build - disabling ARM64 dobby support")
    set(DOBBY_X86_64 ON CACHE BOOL "Enable x86_64 dobby support" FORCE)
    set(DOBBY_ARM64 OFF CACHE BOOL "Disable ARM64 dobby support" FORCE)
    set(DOBBY_ARM32 OFF CACHE BOOL "Disable ARM32 dobby support" FORCE)
    set(DOBBY_ARCH "x86_64" CACHE STRING "Force x86_64 dobby architecture" FORCE)
endif()

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
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_BACKENDS_DIR})
include_directories(${SRC_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${APIS_DIR})

# ======================================================================
# 3. IMGUI SOURCES
# ======================================================================
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)

# ======================================================================
# 4. LUMA SOURCES
# ======================================================================
set(LUMA_SOURCES
    ${SRC_DIR}/luma_linux.c
    ${SRC_DIR}/luma_module_manager.cpp
    ${SRC_DIR}/luma_core.cpp
    ${SRC_DIR}/luma_config.cpp
    ${SRC_DIR}/luma_gui.cpp
    ${SRC_DIR}/luma_hud_editor.cpp
    ${SRC_DIR}/luma_game_api.cpp
    ${SRC_DIR}/luma_camera_api.cpp

    ${APIS_DIR}/bedrock_1_21_30.cpp

    ${SRC_DIR}/modules/luma_autosprint_module.cpp
    ${SRC_DIR}/modules/luma_coords_module.cpp
    ${SRC_DIR}/modules/luma_cps_module.cpp
    ${SRC_DIR}/modules/luma_fps_module.cpp
    ${SRC_DIR}/modules/luma_keystrokes_module.cpp
    ${SRC_DIR}/modules/luma_ping_module.cpp
    ${SRC_DIR}/modules/luma_zoom_module.cpp
)

# ======================================================================
# 5. Dobby (IMPORTED STATIC LIBRARY)
# ======================================================================
if (NOT DEFINED DOBBY_LIBRARY OR NOT DEFINED DOBBY_INCLUDE_DIR)
    message(FATAL_ERROR "DOBBY_LIBRARY and DOBBY_INCLUDE_DIR must be provided")
endif()

add_library(dobby STATIC IMPORTED)
set_target_properties(dobby PROPERTIES
    IMPORTED_LOCATION "${DOBBY_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${DOBBY_INCLUDE_DIR}"
)

# ======================================================================
# 6. BUILD SHARED LIBRARY
# ======================================================================
add_library(luma_client SHARED
    ${LUMA_SOURCES}
    ${IMGUI_SOURCES}
)

# ======================================================================
# 7. LINK LIBRARIES
# ======================================================================
if (ANDROID)
    target_link_libraries(luma_client
        GLESv3
        log
        dl
        dobby
    )
else()
    target_link_libraries(luma_client
        dl
        dobby
    )
endif()

# ======================================================================
# 8. COMPILER FLAGS
# ======================================================================
target_compile_options(luma_client PRIVATE
    -fvisibility=hidden
    -Os
    -ffunction-sections
    -fdata-sections
)

# ======================================================================
# 9. LINKER FLAGS
# ======================================================================
target_link_options(luma_client PRIVATE
    -Wl,--gc-sections
)

# ======================================================================
# 10. OUTPUT CONFIGURATION
# ======================================================================
set_target_properties(luma_client PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    OUTPUT_NAME "luma_client"
)

message(STATUS "==================================================")
message(STATUS "Luma Client Build Configuration")
message(STATUS "==================================================")
message(STATUS "CMake Version:     ${CMAKE_VERSION}")
message(STATUS "C++ Standard:      ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "Architecture:      ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Dobby ARM64:       ${DOBBY_ARM64}")
message(STATUS "Output Binary:     libluma_client.so")
message(STATUS "==================================================")
