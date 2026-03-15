#!/usr/bin/env bash
# install.sh - Build and install Luma Client in one step

set -e
cd "$(dirname "$0")"

echo "[Luma] Building..."
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make -j"$(nproc)"
cd ..

echo "[Luma] Installing..."
MOD_DIR="$HOME/.local/share/mcpelauncher/mods/Luma Client/v2.0.0/x86_64"
mkdir -p "$MOD_DIR"
cp build/libluma_client.so "$MOD_DIR/"
chmod +x "$MOD_DIR/libluma_client.so"

echo "[Luma] Done! Launch with:"
echo "  mcpelauncher-client 2>&1 | grep -E 'Luma|error'"
