# 🌟 Luma Client

<div align="center">

![Build Status](https://github.com/sheep1fy/Luma-Client/workflows/Luma%20Client%20CI%2FCD/badge.svg)
![Platform](https://img.shields.io/badge/platform-Linux-orange.svg)
![Minecraft](https://img.shields.io/badge/minecraft-bedrock%201.21.30-green.svg)

**A client mod for Minecraft Bedrock Edition on Linux**

*Built for Linux users running Bedrock Edition through mcpelauncher*

[Features](#-features) • [Installation](#-installation) • [Building](#-building-from-source) • [Usage](#-usage)

</div>

---

## 🎮 Overview

Luma Client is a client-side modification for Minecraft Bedrock Edition 1.21.30, designed for Linux users running the game through [mcpelauncher](https://github.com/minecraft-linux/mcpelauncher-manifest). It provides HUD modules and quality-of-life features.

---

## ✨ Features

### HUD Modules

- **📊 FPS Counter** - Display your current framerate
- **🌍 Coordinates Display** - Show your X, Y, Z position
- **🖱️ CPS Counter** - Track clicks per second
- **📡 Ping Display** - Monitor your connection latency
- **⌨️ Keystrokes** - Visual display of WASD key presses

### Gameplay Features

- **🏃 AutoSprint** - Automatically sprint when moving forward
- **🔍 Zoom** - Adjustable camera zoom functionality

### Configuration

- **HUD Editor** - Drag and drop HUD elements to customize positions
- **Save/Load Config** - Persistent settings stored in `~/.local/share/mcpelauncher/luma_config.txt`
- **Per-Module Toggle** - Enable/disable individual features

---

## 📦 Installation

### Prerequisites

- Linux operating system
- [mcpelauncher](https://github.com/minecraft-linux/mcpelauncher-manifest) installed
- Minecraft Bedrock Edition 1.21.30

### Quick Install

1. **Download the latest build**
   ```bash
   # Download luma_client.so from GitHub Actions artifacts
   # Or build from source (see below)
   ```

2. **Install to mcpelauncher mods directory**
   ```bash
   mkdir -p ~/.local/share/mcpelauncher/mods
   cp luma_client.so ~/.local/share/mcpelauncher/mods/
   ```

3. **Launch Minecraft**
   ```bash
   mcpelauncher-client
   ```

4. **Open Luma Client GUI**
   - Press `Right Shift + F3` in-game

---

## 🔨 Building from Source

### Build Dependencies

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install -y cmake ninja-build gcc g++ libegl1-mesa-dev libgles2-mesa-dev

# Arch Linux
sudo pacman -S cmake ninja gcc mesa

# Fedora
sudo dnf install cmake ninja-build gcc-c++ mesa-libEGL-devel mesa-libGLES-devel
```

### Build Steps

```bash
# Clone the repository
git clone https://github.com/sheep1fy/Luma-Client.git
cd Luma-Client

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_STANDARD=20

# Build
ninja

# Output: build/luma_client.so
```

### Note for GitHub Actions

The build requires Android NDK for compiling ImGui's Android backend. The CI/CD workflow automatically sets this up.

---

## 🎯 Usage

### Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `Right Shift + F3` | Toggle Luma Client menu |
| `Left Alt` or `Right Mouse` | Zoom (when enabled) |

### Getting Started

1. Launch Minecraft with mcpelauncher
2. Press `Right Shift + F3` to open the menu
3. Check the modules you want to enable
4. Click "HUD Editor" to reposition elements
5. Click "Save Config" to save your settings

### Configuration File

Settings are stored in:
```
~/.local/share/mcpelauncher/luma_config.txt
```

Format:
```
fps_x=10.0
fps_y=10.0
module_FPS=1
module_Coordinates=1
module_AutoSprint=1
...
```

---

## 🏗️ Technical Details

### Technology Stack

- **Language**: C++20
- **UI Framework**: Dear ImGui
- **Graphics**: OpenGL ES2 via EGL
- **Build System**: CMake + Ninja
- **Platform**: Linux x86_64

### Project Structure

```
Luma-Client/
├── src/
│   ├── luma_core.cpp           # Core initialization
│   ├── luma_gui.cpp             # GUI system
│   ├── luma_module_manager.cpp # Module management
│   ├── luma_hud_editor.cpp     # HUD positioning
│   ├── luma_config.cpp          # Configuration
│   ├── luma_game_api.cpp        # Game API hooks
│   ├── luma_camera_api.cpp     # Camera controls
│   ├── luma_linux.cpp           # Linux/mcpelauncher integration
│   └── modules/                 # Feature modules
├── include/                     # Headers
├── apis/                        # Bedrock 1.21.30 API definitions
├── imgui/                       # Dear ImGui library
└── CMakeLists.txt              # Build configuration
```

### Modules Included

All modules are in `src/modules/`:
- `luma_fps_module.cpp`
- `luma_coords_module.cpp`
- `luma_cps_module.cpp`
- `luma_ping_module.cpp`
- `luma_keystrokes_module.cpp`
- `luma_autosprint_module.cpp`
- `luma_zoom_module.cpp`

---

## 🐛 Troubleshooting

### Module not loading

```bash
# Check if the .so is in the correct location
ls ~/.local/share/mcpelauncher/mods/

# Check if mcpelauncher is loading mods
# Look for mod loading messages in console output
```

### GUI not appearing

- Make sure you're pressing **Right Shift + F3** (not Left Shift)
- Check that Luma Client loaded successfully in the console

### Build errors

- Ensure all dependencies are installed
- For Android backend compilation errors, the CI/CD workflow handles NDK setup automatically
- Clean build: `rm -rf build && mkdir build`

---

## 🤝 Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test your changes
5. Submit a pull request

---

## 🙏 Acknowledgments

- **[mcpelauncher](https://github.com/minecraft-linux/mcpelauncher-manifest)** - Making Bedrock Edition work on Linux
- **[Dear ImGui](https://github.com/ocornut/imgui)** - UI library
- **Flarial Client** - Original inspiration

---

<div align="center">

**Built for the Linux Minecraft Bedrock community**

⭐ Star this repo if you find it useful!

</div>
