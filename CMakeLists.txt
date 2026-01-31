cmake_minimum_required(VERSION 3.20)
project(LumaClient LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Prefer GLVND if present, but don't hard-fail CI if OpenGL dev libs are missing
set(OpenGL_GL_PREFERENCE GLVND)

# Try OpenGL, but don't make it REQUIRED to avoid CI failure
find_package(OpenGL QUIET)

# Assume glfw is provided by your environment (vcpkg/system package)
# If you use vcpkg, you probably already have `find_package(glfw3 CONFIG REQUIRED)` elsewhere.
find_package(glfw3 QUIET)

# ImGui locations (adjust if different in your repo)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)

set(LUMA_SOURCES
    src/main.cpp
    src/apis/bedrock_1_21_30.cpp
    ${IMGUI_SOURCES}
)

add_library(luma_client SHARED ${LUMA_SOURCES})

# Ensure loader header is found
target_include_directories(luma_client PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_BACKENDS_DIR}
)

# Match backend expectations: GLFW pulls in GL headers, backend loads functions
target_compile_definitions(luma_client PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM   # use ImGui's built-in loader function
)

# Link what we can detect; don't hard-require OpenGL::GL
if(TARGET glfw OR glfw3_FOUND)
    # vcpkg config mode: glfw
    target_link_libraries(luma_client PRIVATE glfw)
endif()

if(OpenGL_FOUND)
    # If OpenGL dev package is present on the system, use it
    target_link_libraries(luma_client PRIVATE OpenGL::GL)
endif()

# Visibility & PIC (useful for client as a shared lib)
set_target_properties(luma_client PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)
