cmake_minimum_required(VERSION 3.20)
project(luma_client VERSION 1.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCES src/luma_linux.c)
add_library(luma_client SHARED ${SOURCES})

target_compile_options(luma_client PRIVATE
    -O3 -DNDEBUG -fPIC
    -Wall -Wextra
)

target_link_libraries(luma_client PRIVATE log m dl)

set_target_properties(luma_client PROPERTIES
    OUTPUT_NAME "luma_client"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    PREFIX ""
    SUFFIX ".so"
)
