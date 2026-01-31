cmake_minimum_required(VERSION 3.16)

project(LumaClient
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "===========================================")
message(STATUS " Luma Client Build Configuration")
message(STATUS "===========================================")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

# Detect architecture
set(LUMA_ARCH_X86 FALSE)
set(LUMA_ARCH_ARM64 FALSE)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|x86")
    set(LUMA_ARCH_X86 TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(LUMA_ARCH_ARM64 TRUE)
endif()

# Options
option(LUMA_USE_DOBBY "Enable Dobby hook engine where supported" ON)

# Dobby integration
set(LUMA_DOBBY_TARGET "")
if(LUMA_USE_DOBBY AND LUMA_ARCH_X86)
    message(STATUS "Dobby: enabled for x86_64")
    # Assuming Dobby is added as a submodule in external/Dobby
    set(LUMA_DOBBY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/Dobby")
    if(EXISTS "${LUMA_DOBBY_DIR}/CMakeLists.txt")
        add_subdirectory(${LUMA_DOBBY_DIR} EXCLUDE_FROM_ALL)
        # Most Dobby forks export a static target named Dobby or dobby
        if(TARGET Dobby)
            set(LUMA_DOBBY_TARGET Dobby)
        elseif(TARGET dobby)
            set(LUMA_DOBBY_TARGET dobby)
        elseif(TARGET dobby_static)
            set(LUMA_DOBBY_TARGET dobby_static)
        else()
            message(FATAL_ERROR "Dobby added, but no known Dobby target (Dobby/dobby/dobby_static) found")
        endif()
    else()
        message(FATAL_ERROR "Dobby directory not found at ${LUMA_DOBBY_DIR}. "
                            "Add Dobby as a submodule or adjust LUMA_DOBBY_DIR.")
    endif()
else()
    message(STATUS "Dobby: disabled for this architecture")
endif()

# Sources
set(LUMA_CLIENT_SOURCES
    src/luma_client.cpp
    # add other source files here
)

add_library(libluma_client SHARED ${LUMA_CLIENT_SOURCES})
set_target_properties(libluma_client PROPERTIES
    OUTPUT_NAME "luma_client"
)

target_include_directories(libluma_client
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(LUMA_DOBBY_TARGET)
    target_link_libraries(libluma_client PRIVATE ${LUMA_DOBBY_TARGET})
endif()

# System libs if needed
# target_link_libraries(libluma_client PRIVATE dl pthread)

message(STATUS "Output Binary: libluma_client.so")
message(STATUS "===========================================")
