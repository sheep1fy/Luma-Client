cmake_minimum_required(VERSION 3.18.1)
project(LumaClient VERSION 1.0.1 LANGUAGES C CXX)

# Global flags
add_compile_definitions(
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
    IMGUI_IMPL_OPENGL_ES3=0
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM=1
    luma_client_EXPORTS
)

# ImGui static library
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC imgui imgui/backends)

# Instead of GLOB_RECURSIVE, do two explicit globs and combine
file(GLOB SRC_FILES "src/*.c" "src/*.cpp")
file(GLOB MODULE_FILES "src/modules/*.cpp")
list(APPEND LUMA_SOURCES ${SRC_FILES} ${MODULE_FILES})

# Main Luma Client - all sources + ImGui
add_library(luma_client SHARED 
    ${LUMA_SOURCES}
)

target_include_directories(luma_client PRIVATE
    .
    include
    src
    src/modules
    imgui
)

target_link_libraries(luma_client 
    imgui
    GLESv3
    log
    dl
    atomic
    m
)

if(ANDROID)
    target_compile_options(luma_client PRIVATE
        -g -DANDROID -fdata-sections -ffunction-sections 
        -funwind-tables -fstack-protector-strong -no-canonical-prefixes 
        -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security 
        -O3 -DNDEBUG -fPIC -march=x86-64 -flto -fvisibility=hidden
    )
endif()

set_target_properties(luma_client PROPERTIES
    OUTPUT_NAME "luma_client"
    POSITION_INDEPENDENT_CODE ON
)
