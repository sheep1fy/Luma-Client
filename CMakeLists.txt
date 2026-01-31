cmake_minimum_required(VERSION 3.18.1)

project(Luma-Client
    VERSION 1.0.1
    DESCRIPTION "Luma Client - Android Bedrock Client"
    LANGUAGES C CXX
)

# Android NDK Detection
if(NOT DEFINED ANDROID)
    if(DEFINED ENV{ANDROID_ABI})
        set(ANDROID TRUE)
    endif()
endif()

if(ANDROID)
    message(STATUS "🎮 Building for Android (MCPE)")
    
    # Find Android log library
    find_library(log-lib log)
    
    # Global compile options for Android
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1 -fno-rtti -fno-exceptions")
    
else()
    message(WARNING "❌ Android not detected - skipping JNI sources")
endif()

# ImGui
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC imgui imgui/backends)

# Luma Client (Main Library)
add_library(luma_client SHARED
    src/luma_linux.c
    src/luma_module_manager.cpp  # Add your other sources here
)

target_include_directories(luma_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

target_link_libraries(luma_client
    imgui
    GLESv3
    dl  # dlsym
)

if(ANDROID)
    target_link_libraries(luma_client ${log-lib})
    target_compile_definitions(luma_client PRIVATE LUMA_CLIENT_ANDROID=1)
endif()

# Optimization flags
target_compile_options(luma_client PRIVATE
    -O3
    -march=native
    -flto
    -fvisibility=hidden
)

set_target_properties(luma_client PROPERTIES
    OUTPUT_NAME "luma_client"
    POSITION_INDEPENDENT_CODE ON
)
