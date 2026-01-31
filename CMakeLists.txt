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

# Dobby handling - optional and architecture-specific
option(LUMA_USE_DOBBY "Enable Dobby hooking library" ${IS_X86_64})
set(LUMA_DOBBY_TARGET "")

if(LUMA_USE_DOBBY AND IS_X86_64)
    message(STATUS "Checking for Dobby...")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/Dobby/CMakeLists.txt")
        message(STATUS "Dobby found, building...")
        add_subdirectory(external/Dobby EXCLUDE_FROM_ALL)
        
        # Try common Dobby target names
        if(TARGET dobby)
            set(LUMA_DOBBY_TARGET dobby)
        elseif(TARGET Dobby)
            set(LUMA_DOBBY_TARGET Dobby)
        elseif(TARGET dobby_static)
            set(LUMA_DOBBY_TARGET dobby_static)
        endif()
        
        if(LUMA_DOBBY_TARGET)
            message(STATUS "Dobby target: ${LUMA_DOBBY_TARGET}")
        else()
            message(WARNING "Dobby built but no compatible target found")
        endif()
    else()
        message(WARNING "Dobby directory missing: external/Dobby/CMakeLists.txt")
        message(WARNING "Disabling Dobby - add with: git submodule add https://github.com/jmpews/Dobby external/Dobby")
    endif()
else()
    message(STATUS "Dobby: disabled (${CMAKE_SYSTEM_PROCESSOR})")
endif()

# Your source files (update these paths to match your actual files)
file(GLOB_RECURSE LUMA_SOURCES 
    src/*.cpp 
    src/*.c
    *.cpp 
    *.c
)

# Create shared library
add_library(luma_client SHARED ${LUMA_SOURCES})

# Includes
target_include_directories(luma_client PRIVATE 
    include/
    src/
    # add your include dirs here
)

# Link Dobby if available
if(LUMA_DOBBY_TARGET)
    target_link_libraries(luma_client PRIVATE ${LUMA_DOBBY_TARGET})
endif()

# Common Linux libraries for a Minecraft client mod
target_link_libraries(luma_client PRIVATE 
    dl 
    pthread 
    m
)

set_target_properties(luma_client PROPERTIES 
    OUTPUT_NAME "luma_client"
    CXX_VISIBILITY_PRESET hidden
)

message(STATUS "Output: luma_client.so")
message(STATUS "===============================================")
