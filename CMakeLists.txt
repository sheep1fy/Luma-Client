cmake_minimum_required(VERSION 3.10)
project(LumaClient)

set(CMAKE_CXX_STANDARD 17)

# Add paths for ImGui and your source
include_directories(include imgui)

# Find necessary Android/Linux libraries
find_library(log-lib log)
find_library(GLESv3-lib GLESv3)

# Define your source files
set(SOURCE_FILES 
    src/main.cpp 
    imgui/imgui.cpp 
    imgui/imgui_draw.cpp 
    imgui/imgui_widgets.cpp 
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_library(luma_client SHARED ${SOURCE_FILES})

# Link libraries
target_link_libraries(luma_client ${log-lib} ${GLESv3-lib})
