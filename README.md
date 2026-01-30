<div align="center">

# 🌟 Luma Client

### A High-Performance Utility Mod for Minecraft Bedrock Edition on Linux

[![Build Status](https://img.shields.io/github/actions/workflow/status/sheep1fy/Luma-Client/build.yml?style=for-the-badge)](https://github.com/sheep1fy/Luma-Client)
[![Platform](https://img.shields.io/badge/platform-Linux-blue?style=for-the-badge)](https://github.com/sheep1fy/Luma-Client)
[![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)](LICENSE)

<img src="https://img.shields.io/badge/Minecraft-1.21.30-brightgreen?style=for-the-badge&logo=minecraft" alt="Minecraft Version"/>
<img src="https://img.shields.io/badge/ImGui-Powered-orange?style=for-the-badge" alt="ImGui"/>

---

**[Features](#-features)** • **[Installation](#-installation)** • **[Building](#-building-from-source)** • **[Contributing](#-contributing)**

</div>

---

## 📖 Overview

Luma Client is a modular, performance-first modification for Minecraft Bedrock Edition on Linux via the mcpelauncher. Built with direct low-level hooking using the Dobby library, Luma provides a suite of utility modules and customizable HUD overlays without impacting game stability or framerate.

### Key Highlights

- **🎯 Zero Performance Impact** - Optimized hooks and render logic
- **🎨 Modern UI** - Sleek ImGui-based interface with Flarial-inspired theming
- **🔧 Modular Architecture** - Easy-to-extend module system
- **🖥️ OpenGL ES 3** - Native mobile rendering backend
- **⚡ Version Targeted** - Built for Bedrock 1.21.30

---

## ✨ Features

### 🎮 Gameplay Modules

| Module | Description | Status |
|--------|-------------|--------|
| **ToggleSprint** | Automatically maintain sprint without holding the key | ✅ Active |
| **Zoom** | Adjustable field-of-view modification for scouting | ✅ Active |

### 📊 HUD Overlays

| Module | Description | Status |
|--------|-------------|--------|
| **FPS Counter** | Real-time framerate display with smoothing | ✅ Active |
| **Coordinates** | Live world position (X, Y, Z) tracking | ✅ Active |
| **CPS Counter** | Left and right click-per-second tracking | ✅ Active |
| **Ping Display** | Network latency monitoring | ✅ Active |
| **Keystrokes** | Visual WASD and mouse input overlay | ✅ Active |

### 🎨 Interface

- **Modern Menu System** - Press `K` to open the configuration interface
- **Custom Theming** - Flarial-inspired cyan accent with glassy dark backgrounds
- **Tabbed Navigation** - Organized sections for Modules, Visuals, Configs, and Info
- **Custom Toggle Switches** - Beautiful animated UI components

---

## 📦 Installation

### Prerequisites

- **Linux Operating System** (Ubuntu, Arch, Fedora, etc.)
- **mcpelauncher-client** installed and configured
- Minecraft Bedrock Edition compatible with 1.21.30

### Quick Install

1. **Locate the Mods Directory**
   ```bash
   cd ~/.local/share/mcpelauncher/mods/
   ```

2. **Download or Build the Mod**
   - Download the latest `libluma_client.so` from [Releases](https://github.com/sheep1fy/Luma-Client/releases)
   - Or [build from source](#-building-from-source)

3. **Install the Shared Library**
   ```bash
   cp libluma_client.so ~/.local/share/mcpelauncher/mods/
   ```

4. **Launch Minecraft**
   ```bash
   mcpelauncher-client
   ```

5. **Open Luma Menu**
   - Press `K` in-game to access the configuration interface

---

## 🔨 Building from Source

### Build Requirements

- **CMake** 3.10 or higher
- **Android NDK** r26b+ (required for cross-compilation)
- **Ninja** or **Make** build system
- **GCC/Clang** with C++17 support

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/sheep1fy/Luma-Client.git
cd Luma-Client

# Create build directory
mkdir build && cd build

# Configure with Android NDK
cmake .. -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=x86_64 \
  -DANDROID_PLATFORM=android-24 \
  -DCMAKE_BUILD_TYPE=Release

# Build
ninja
```

The compiled `libluma_client.so` will be located in the `build/` directory.

### Manual Build (Alternative)

```bash
# Configure without Android NDK (for development)
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

---

## 🏗️ Project Architecture

### Directory Structure

```
Luma-Client/
├── .github/
│   └── workflows/
│       └── build.yml          # CI/CD pipeline configuration
├── apis/
│   ├── bedrock_1_21_30.cpp    # Game API implementation for 1.21.30
│   └── bedrock_1_21_30.hpp    # API headers and type definitions
├── imgui/                     # Dear ImGui library
│   ├── backends/              # OpenGL ES 3 and Android backends
│   └── *.cpp/*.h              # Core ImGui implementation
├── include/                   # Public API headers
│   ├── luma_camera_api.h      # Camera control interface
│   ├── luma_config.h          # Configuration management
│   ├── luma_game_api.h        # Game interaction interface
│   ├── luma_gui.h             # GUI system
│   ├── luma_hud.h             # HUD overlay system
│   ├── luma_module.h          # Module base interface
│   └── luma_module_manager.h  # Module registry
├── src/
│   ├── modules/               # Feature implementations
│   │   ├── luma_autosprint_module.cpp
│   │   ├── luma_coords_module.cpp
│   │   ├── luma_cps_module.cpp
│   │   ├── luma_fps_module.cpp
│   │   ├── luma_keystrokes_module.cpp
│   │   ├── luma_ping_module.cpp
│   │   └── luma_zoom_module.cpp
│   ├── luma_linux.c           # Entry point and Dobby hook setup
│   ├── luma_module_manager.cpp # Core module registry and UI
│   └── luma_core.cpp          # Core initialization logic
├── CMakeLists.txt             # Build system configuration
└── README.md                  # This file
```

### Technology Stack

- **Language**: C++17, C11
- **Graphics**: OpenGL ES 3.0
- **UI Framework**: Dear ImGui 1.x
- **Hooking**: Dobby (PLT/GOT hooking)
- **Build System**: CMake 3.10+
- **Target Platform**: Android x86_64 via mcpelauncher

### Module System

Modules follow a simple lifecycle pattern:

```cpp
struct LumaModule {
    const char* name;
    bool enabled;
    void (*on_tick)(float dt);    // Update logic
    void (*on_render)();          // Draw calls
};
```

Each module is self-contained and registered via the module manager.

---

## 🤝 Contributing

Contributions are welcome! Whether it's bug fixes, new modules, or documentation improvements, your help is appreciated.

### How to Contribute

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/AmazingFeature`)
3. **Commit** your changes (`git commit -m 'Add some AmazingFeature'`)
4. **Push** to the branch (`git push origin feature/AmazingFeature`)
5. **Open** a Pull Request

### Adding New Modules

1. Create your module file in `src/modules/luma_yourmodule_module.cpp`
2. Implement the `LumaCreate*Module()` function
3. Register it in `luma_module_manager.cpp`
4. Follow the existing module patterns for consistency

### Code Style

- Use consistent indentation (spaces, not tabs)
- Follow existing naming conventions (`snake_case` for functions, `PascalCase` for types)
- Comment complex logic and hook offsets
- Test thoroughly before submitting

---

## 📋 Technical Details

### Game API Integration

Luma uses versioned API wrappers to interact with Minecraft Bedrock internals:

- **Dynamic symbol resolution** via `dlsym`
- **Offset-based memory access** for player data, camera, and network handlers
- **Safe fallbacks** when API calls fail

### Rendering Pipeline

1. **Hook Injection** - Dobby hooks intercept the game's render loop
2. **ImGui Context** - Separate rendering context for overlays
3. **Background Draw List** - Non-intrusive HUD elements
4. **OpenGL ES Blending** - Alpha-blended UI with minimal overdraw

### Performance Optimizations

- **Exponential smoothing** for FPS calculation (90/10 split)
- **Minimal state changes** in render loop
- **Lazy initialization** of OpenGL resources
- **Batch rendering** via ImGui draw lists

---

## 🐛 Known Issues

- Some offsets in `bedrock_1_21_30.cpp` are placeholders and need reverse engineering
- FOV modification may not persist across game restarts
- Ping display currently returns 0 (network handler not fully hooked)

---

## 📜 License

This project is open source and available under the [MIT License](LICENSE).

---

## 🙏 Acknowledgments

- **[Dear ImGui](https://github.com/ocornut/imgui)** - Bloat-free immediate mode GUI
- **[Dobby](https://github.com/jmpews/Dobby)** - Lightweight hooking framework
- **[mcpelauncher](https://github.com/minecraft-linux/mcpelauncher-manifest)** - Linux Bedrock launcher
- **Flarial Client** - UI design inspiration

---

<div align="center">

**Built with ❤️ for the Linux Minecraft Community**

Maintained by [sheep1fy](https://github.com/sheep1fy)

⭐ **Star this repo if you find it useful!** ⭐

</div>
