🌟 Luma Client

<div align="center">

A high-performance, glassmorphism-inspired client for Minecraft Bedrock Edition on Linux.

Engineered for competitive PvP through mcpelauncher integration.

✨ Features • 📦 Installation • 🔨 Building • 🎯 Usage

</div>
🎮 Overview

Luma Client is a professional-grade modification for Minecraft Bedrock, specifically optimized for the Linux mcpelauncher environment. Unlike standard mods, Luma uses a C-to-C++ bridge with Dobby hooking to provide a lag-free experience with a modern, sleek interface inspired by Flarial.
✨ Features
💎 The "Flarial" Modern UI

    Glassmorphism: Semi-transparent, blurred-background menu for a premium feel.

    Sidebar Navigation: Organized tabs for Modules, Visuals, and Settings.

    Animated Toggles: Custom-rendered Cyan switches replacing standard checkboxes.

🛡️ PvP HUD Modules

    ⌨️ Keystrokes: High-contrast WASD & Mouse button display.

    🖱️ CPS Counter: Real-time Click-Per-Second tracking for both buttons.

    🏃 ToggleSprint: Maintain maximum speed without fatigue.

    📊 Performance Stats: Sleek FPS, Ping, and Coordinate displays.

⚙️ Engine Features

    Low Latency: Hooked via Dobby directly into the GLES3 render loop.

    Persistent Config: Settings save automatically to your local share directory.

🏗️ Technical Architecture

Luma uses a hybrid architecture to ensure compatibility with the Linux Bedrock launcher:

    luma_linux.c: The entry point. Handles low-level Dobby hooks for input and rendering.

    luma_module_manager.cpp: The brain. Manages UI state, modern styling, and module logic.

    luma_theme.hpp: Defines the "Sleek Dark" color palette and rounding constants.

📦 Installation
Quick Setup

    Prepare Directory
    Bash

    mkdir -p ~/.local/share/mcpelauncher/mods

    Deploy Binary
    Move the compiled libluma_client.so into the folder above.

    Launch
    Open Minecraft via mcpelauncher-client and press K to toggle the menu.

🔨 Building from Source
1. Prerequisites

Ensure you have the Android NDK (r27+) and CMake installed.
2. Compile
Bash

git clone https://github.com/sheep1fy/Luma-Client.git
cd Luma-Client
mkdir build && cd build

# Configure for Android/Linux target
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

🎯 Usage
Key	Action
K	Toggle Modern Mod Menu
WASD	Movement (Updates Keystrokes HUD)
ESC	Close Menu / Release Mouse
🤝 Contributing

Luma is open-source. We welcome UI/UX improvements and new PvP modules!

    Fork the repo.

    Create your module in src/modules/.

    Open a Pull Request.

<div align="center">

Built with ❤️ for the Linux Bedrock Community

⭐ Star this repository if you love the sleek UI!

</div>
