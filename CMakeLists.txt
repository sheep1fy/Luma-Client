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

# ONLY Linux-compatible ImGui files
set(IMGUI_CORE
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)

set(IMGUI_LINUX_BACKEND
    imgui/backends/imgui_impl_opengl3.cpp
)

# Core project sources (auto-find)
file(GLOB_RECURSE LUMA_SOURCES 
    "src/*.cpp"
    "apis/*.cpp"
    "*.cpp"
)

# Linux-only: exclude Android/iOS backends
list(FILTER LUMA_SOURCES EXCLUDE REGEX ".*(android|ios|win32|dx).*")

# Combine all sources
list(APPEND LUMA_SOURCES ${IMGUI_CORE} ${IMGUI_LINUX_BACKEND})

# Remove duplicates
list(REMOVE_DUPLICATES LUMA_SOURCES)

list(LENGTH LUMA_SOURCES source_count)
message(STATUS "Found ${source_count} Linux-compatible sources")

if(source_count EQUAL 0)
    message(FATAL_ERROR "No sources found! Add .cpp files to src/ or apis/")
endif()

add_library(luma_client SHARED ${LUMA_SOURCES})

target_include_directories(luma_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

# Linux essentials
target_link_libraries(luma_client PRIVATE 
    dl 
    pthread 
    m
)

# Dobby (optional)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/Dobby/CMakeLists.txt")
    add_subdirectory(external/Dobby EXCLUDE_FROM_ALL)
    if(TARGET dobby)
        target_link_libraries(luma_client PRIVATE dobby)
    endif()
endif()

# OpenGL loader definitions (fixes imgui_impl_opengl3_loader.h)
target_compile_definitions(luma_client PRIVATE 
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

set_target_properties(luma_client PROPERTIES 
    OUTPUT_NAME "luma_client"
    CXX_VISIBILITY_PRESET hidden
)

message(STATUS "✅ luma_client.so ready for Linux!")
message(STATUS "===============================================")
