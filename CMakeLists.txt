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

# Auto-find ALL your actual source files
file(GLOB_RECURSE LUMA_SOURCES 
    "src/*.cpp"
    "apis/*.cpp"
    "imgui/*.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
)

# Remove demo if present (optional, saves space)
list(FILTER LUMA_SOURCES EXCLUDE REGEX "imgui_demo.cpp$")

# Verify we found sources
list(LENGTH LUMA_SOURCES source_count)
if(source_count EQUAL 0)
    message(FATAL_ERROR "No source files found! Check your src/ apis/ imgui/ directories")
endif()
message(STATUS "Found ${source_count} source files")

add_library(luma_client SHARED ${LUMA_SOURCES})

target_include_directories(luma_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

# Essential Linux libraries
target_link_libraries(luma_client PRIVATE dl pthread m)

# Dobby (if exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/Dobby/CMakeLists.txt")
    add_subdirectory(external/Dobby EXCLUDE_FROM_ALL)
    if(TARGET dobby)
        target_link_libraries(luma_client PRIVATE dobby)
        message(STATUS "Dobby: linked")
    endif()
endif()

# OpenGL - optional
find_package(OpenGL QUIET)
if(OpenGL_FOUND)
    target_link_libraries(luma_client PRIVATE OpenGL::GL)
    message(STATUS "OpenGL: enabled")
else()
    message(STATUS "OpenGL: disabled (headless build)")
endif()

set_target_properties(luma_client PROPERTIES 
    OUTPUT_NAME "luma_client"
    CXX_VISIBILITY_PRESET hidden
)

message(STATUS "SUCCESS: luma_client.so will be built!")
message(STATUS "===============================================")
