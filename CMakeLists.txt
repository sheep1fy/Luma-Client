cmake_minimum_required(VERSION 3.10)
project(luma_client)

set(CMAKE_CXX_STANDARD 17)

# 1. Define Paths
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 2. Include Directories
include_directories(${SRC_DIR})
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)

# 3. Collect ImGui Sources (Required to fix your "file not found" error)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# 4. Define Luma Sources
set(LUMA_SOURCES
    ${SRC_DIR}/luma_linux.c
    ${SRC_DIR}/luma_module_manager.cpp
)

# 5. Build Shared Library
add_library(luma_client SHARED ${LUMA_SOURCES} ${IMGUI_SOURCES})

# 6. Link Libraries
# GLESv3 is used for mobile/bedrock rendering
target_link_libraries(luma_client GLESv3 log)
