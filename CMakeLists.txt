cmake_minimum_required(VERSION 3.20)
project(luma_client VERSION 1.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCES src/luma_linux.c)
add_library(luma_client SHARED ${SOURCES})

target_compile_options(luma_client PRIVATE
    -O3 -DNDEBUG -fPIC
    -fno-stack-protector -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -Wall -Wextra
)

# FIXED: NO -llog! Just dl + z
target_link_libraries(luma_client PRIVATE dl z)

set_target_properties(luma_client PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Luma Client/1.0.1/x86_64"
)
