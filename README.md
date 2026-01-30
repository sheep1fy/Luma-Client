🌟 Luma Client

<div align="center">

A high-performance utility mod for Minecraft Bedrock Edition on Linux. Integrated via mcpelauncher using low-level hooks for maximum efficiency.

Key Modules • Installation • Compiling • Architecture

</div>

🎮 Overview

Luma Client is a modular modification for the Linux-based Minecraft Bedrock launcher. It utilizes the Dobby hooking library to interface directly with the game's internal functions, providing a customizable HUD and utility modules. The client is designed with a "performance-first" mindset, ensuring that overlays and logic do not impact game stability or framerate.

🛠 Key Modules

The client is built on a modular registry, allowing for clean toggling and management of features.

⌨️ Keystrokes: Visualizes WASD and mouse inputs with configurable positioning.

🖱️ CPS Counter: Real-time tracking for Left and Right click speeds.

🏃 ToggleSprint: Keeps the player in a sprinting state without holding the key.

📊 Status HUD: Sleek overlays for FPS, Ping, and World Coordinates.

🔍 Zoom: Adjustable field-of-view modification for scouting.

🏗 Project Structure

The codebase is organized to maintain a strict separation between the Linux entry point and the cross-platform module logic.

Luma-Client/
├── src/
│   ├── luma_linux.c           # Entry point & Dobby hook definitions
│   ├── luma_module_manager.cpp # Core logic and UI registry
│   ├── luma_module_manager.hpp # Bridge header for C/C++ interoperability
│   └── modules/               # Implementation files for each feature
├── include/                   # Shared headers and API definitions
├── imgui/                     # Dear ImGui source (Render Abstraction)
└── CMakeLists.txt             # Unified build system configuration


📦 Installation

Locate Mods Folder: Open your file manager and navigate to ~/.local/share/mcpelauncher/mods/.

Add Binary: Place the compiled libluma_client.so file into this directory.

Launch Game: Start Minecraft via mcpelauncher-client.

Open Menu: Press K (default) to open the Luma configuration interface.

🔨 Building from Source

Prerequisites

CMake 3.10+

Android NDK (r27+): Essential for cross-compiling for the launcher environment.

Make or Ninja

Steps

# Clone the repository
git clone [https://github.com/sheep1fy/Luma-Client.git](https://github.com/sheep1fy/Luma-Client.git)
cd Luma-Client

# Create and enter build directory
mkdir build && cd build

# Configure and compile
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)


The resulting .so file will be found in the build/ directory.

🤝 Contributing

Contributions are welcome. Please ensure that any new modules are registered within luma_module_manager.cpp and follow the existing documentation style.

<div align="center">

Built for the Linux Minecraft Community Maintained by sheep1fy

</div>
