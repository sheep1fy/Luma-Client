cmake_minimum_required(VERSION 3.16)
project(LumaClient VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "===============================================")
message(STATUS " Luma Client Build Configuration")
message(STATUS "===============================================")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

# Architecture detection
set(IS_X86_64 FALSE)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|i.86")
    set(IS_X86_64 TRUE)
endif()

# Dobby handling
option(LUMA_USE_DOBBY "Enable Dobby hooking library" ${IS_X86_64})
set(LUMA_DOBBY_TARGET "")

if(LUMA_USE_DOBBY AND IS_X86_64)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/Dobby/CMakeLists.txt")
        message(STATUS "Dobby found, building...")
        add_subdirectory(external/Dobby EXCLUDE_FROM_ALL)
        
        if(TARGET dobby OR TARGET Dobby OR TARGET dobby_static)
            message(STATUS "Dobby: enabled")
        else()
            message(WARNING "Dobby built but no target found")
        endif()
    else()
        message(WARNING "Dobby missing - continuing without")
    endif()
endif()

# EXPLICIT source lists - no more globbing issues
set(LUMA_SOURCES
    # Your main sources
    src/main.cpp                    # adjust to your actual entry point
    apis/bedrock_1_21_30.cpp
    
    # ImGui core (REQUIRED)
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp           # optional but usually included
    
    # ImGui backends (only ones you actually use)
    imgui/backends/imgui_impl_opengl3.cpp
    # imgui/backends/imgui_impl_android.cpp  # comment out if not needed for Linux
)

add_library(luma_client SHARED ${LUMA_SOURCES})

# FIXED include directories - imgui/ is now included
target_include_directories(luma_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui           # ← FIXES imgui.h missing
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends  # ← backend headers
)

# Link Dobby if available
if(TARGET dobby)
    target_link_libraries(luma_client PRIVATE dobby)
elseif(TARGET Dobby)
    target_link_libraries(luma_client PRIVATE Dobby)
elseif(TARGET dobby_static)
    target_link_libraries(luma_client PRIVATE dobby_static)
endif()

# Essential Linux libraries
target_link_libraries(luma_client PRIVATE dl pthread m)

# OpenGL (for imgui_impl_opengl3)
find_package(OpenGL REQUIRED)
target_link_libraries(luma_client PRIVATE OpenGL::GL)

set_target_properties(luma_client PROPERTIES 
    OUTPUT_NAME "luma_client"
    CXX_VISIBILITY_PRESET hidden
)

message(STATUS "Output: luma_client.so")
message(STATUS "===============================================")
