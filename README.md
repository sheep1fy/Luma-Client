# Luma Client v2.0.0

A feature-rich mod for **Minecraft Bedrock Edition** on Linux (`mcpelauncher-client`).

Provides HUD modules, performance monitoring, and QoL enhancements via version-independent OpenGL + SDL2 hooking.

---

## Features

| Module | Description |
|---|---|
| FPS Counter | Smooth 30-frame rolling average, colour-coded |
| Coordinates | Live XYZ display |
| CPS Counter | Left/right clicks per second (1 s rolling window) |
| Keystrokes | WASD + LMB/RMB visualizer |
| Ping | Server latency with colour indicator |
| Zoom | Hold **C** to zoom (configurable factor, smooth lerp) |
| Auto Sprint | Automatic sprint when W is held |
| HUD Editor | Drag-and-drop module repositioning |
| ImGui Menu | Full settings UI — toggle with **K** |

---

## Requirements

```bash
sudo pacman -S cmake base-devel libx11 sdl2   # Arch / your system
# or
sudo apt install cmake build-essential libx11-dev libsdl2-dev
```

---

## Build

```bash
git clone https://github.com/sheep1fy/Luma-Client
cd Luma-Client
git submodule update --init --recursive   # pulls ImGui

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

---

## Install

```bash
make install_mod   # copies to mcpelauncher mods directory automatically
```

Or manually:

```bash
MOD_DIR="$HOME/.local/share/mcpelauncher/mods/Luma Client/v2.0.0/x86_64"
mkdir -p "$MOD_DIR"
cp build/libluma_client.so "$MOD_DIR/"
chmod +x "$MOD_DIR/libluma_client.so"
```

---

## Launch & Verify

```bash
mcpelauncher-client 2>&1 | grep -E 'Luma|error'
```

Expected output:
```
[LumaClient] Shared library loaded - constructor called
[LumaClient] Registering modules...
[LumaClient] All modules registered
[LumaClient] glClear hook ACTIVE
[LumaClient] Using SDL2 event hooking for keys
[LumaClient] Hooked SDL_PollEvent successfully
```

Press **K** in-game to open the menu.

---

## How It Works

### Rendering hook (version-independent)
```
mcpelauncher → libminecraftpe.so → glClear()
                                       ↑
                          libluma_client.so intercepts via RTLD_NEXT
                          → renders ImGui overlay
                          → calls original glClear
```

### Input hook (Wayland + X11 compatible)
```
Wayland/X11 → SDL2 (SDL_PollEvent) → Minecraft
                        ↑
              libluma_client.so intercepts
              → checks for K / C / WASD keys
              → passes event to game unchanged
```

---

## Diagnostics

```bash
chmod +x luma_diagnostic.sh
./luma_diagnostic.sh
```

Test X11 key detection standalone:
```bash
gcc x11_key_test.c -o x11_key_test -lX11
./x11_key_test
```

---

## Adding a New Module

1. Create `src/modules/luma_myfeature_module.cpp`
2. Define a `static LumaModule g_myfeature = { ... };`
3. Implement `void luma_myfeature_module_register() { LumaRegisterModule(&g_myfeature); }`
4. Declare it in `src/luma_core.cpp` and call it from `luma_core_init()`
5. Add the file to `MODULE_SOURCES` in `CMakeLists.txt`

---

## License

MIT — see [LICENSE](LICENSE)

**Credits:** ImGui by Omar Cornut · mcpelauncher by MCMrARM · Original Luma Client by sheep1fy
