cmake_minimum_required(VERSION 3.14)
project(luma_client VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Optionally set build type to Release by default if not set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Add source files
set(SOURCES
    src/main.cpp
    src/imgui_impl.cpp
    # Add any other source files here
)

add_library(luma_client SHARED ${SOURCES})

# Include directories (adjust if needed)
target_include_directories(luma_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
)

# Link libraries (adjust if needed)
target_link_libraries(luma_client
    # Link other dependencies here if any
    m  # link math lib, but static linking below will inline it
)

# This is the key part: static link libm to avoid runtime dependency on libm.so.6
target_link_options(luma_client PRIVATE "-Wl,-Bstatic" "-lm" "-Wl,-Bdynamic")

# Optional: Additional linker flags to prevent undefined symbols
target_link_options(luma_client PRIVATE "-Wl,--no-undefined")

# Set output directory for built .so (optional)
set_target_properties(luma_client PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/x86_64"
)

