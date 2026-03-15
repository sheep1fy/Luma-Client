#!/usr/bin/env bash
# luma_diagnostic.sh - System check for Luma Client installation

set -euo pipefail
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; NC='\033[0m'

ok()   { echo -e "  ${GREEN}✅${NC} $1"; }
warn() { echo -e "  ${YELLOW}⚠️ ${NC} $1"; }
fail() { echo -e "  ${RED}❌${NC} $1"; }

echo ""
echo "════════════════════════════════════════"
echo "  Luma Client v2.0.0 - Diagnostic"
echo "════════════════════════════════════════"
echo ""

MOD_PATH="$HOME/.local/share/mcpelauncher/mods/Luma Client/v2.0.0/x86_64/libluma_client.so"

# ── Session type ──────────────────────────────────────────────
echo "[ System ]"
SESSION="${XDG_SESSION_TYPE:-unknown}"
echo "  Session type: $SESSION"
if [[ "$SESSION" == "wayland" ]]; then
    ok "Wayland detected - SDL2 hook mode required (default)"
elif [[ "$SESSION" == "x11" ]]; then
    ok "X11 detected - both X11 and SDL2 hooks available"
else
    warn "Unknown session type: $SESSION"
fi

# ── DISPLAY ──────────────────────────────────────────────────
echo ""
echo "[ Display ]"
if [[ -n "${DISPLAY:-}" ]]; then
    ok "DISPLAY=$DISPLAY"
else
    warn "DISPLAY not set (may be Wayland-only)"
fi

# ── X11 libraries ────────────────────────────────────────────
echo ""
echo "[ X11 Libraries ]"
if ldconfig -p 2>/dev/null | grep -q "libX11"; then
    ok "libX11 found"
else
    warn "libX11 not found - X11 diagnostics unavailable"
fi

# ── SDL2 ─────────────────────────────────────────────────────
echo ""
echo "[ SDL2 ]"
if ldconfig -p 2>/dev/null | grep -q "libSDL2"; then
    ok "libSDL2 found"
else
    fail "libSDL2 not found - install libsdl2-dev"
fi

# ── Mod file ─────────────────────────────────────────────────
echo ""
echo "[ Mod Installation ]"
if [[ -f "$MOD_PATH" ]]; then
    ok "Mod file exists: $MOD_PATH"
    if [[ -x "$MOD_PATH" ]]; then
        ok "Mod file is executable"
    else
        warn "Mod file not executable - run: chmod +x \"$MOD_PATH\""
    fi

    # Dependencies
    echo ""
    echo "[ Shared Library Dependencies ]"
    if command -v ldd &>/dev/null; then
        ldd "$MOD_PATH" 2>&1 | while read -r line; do
            if echo "$line" | grep -q "not found"; then
                fail "$line"
            else
                echo "    $line"
            fi
        done
    else
        warn "ldd not available"
    fi

    # Symbols
    echo ""
    echo "[ Exported Symbols ]"
    if nm -D "$MOD_PATH" 2>/dev/null | grep -q "glClear"; then
        ok "glClear hook exported"
    else
        warn "glClear not found in symbol table"
    fi
    if nm -D "$MOD_PATH" 2>/dev/null | grep -q "SDL_PollEvent"; then
        ok "SDL_PollEvent hook exported"
    else
        warn "SDL_PollEvent not in symbol table (may still work via RTLD_NEXT)"
    fi
else
    fail "Mod file NOT found: $MOD_PATH"
    echo ""
    echo "  To install:"
    echo "    mkdir -p ~/.local/share/mcpelauncher/mods/Luma\\ Client/v2.0.0/x86_64/"
    echo "    cp build/libluma_client.so \"$MOD_PATH\""
fi

# ── CMake ────────────────────────────────────────────────────
echo ""
echo "[ Build Tools ]"
if command -v cmake &>/dev/null; then
    CMAKE_VER=$(cmake --version | head -1)
    ok "cmake: $CMAKE_VER"
else
    fail "cmake not found - install: sudo apt install cmake"
fi
if command -v make &>/dev/null; then
    ok "make found"
fi
if command -v g++ &>/dev/null; then
    ok "g++ found"
fi

echo ""
echo "════════════════════════════════════════"
echo "  Diagnostic complete"
echo "════════════════════════════════════════"
echo ""
echo "Launch command:"
echo "  mcpelauncher-client 2>&1 | grep -E 'Luma|error'"
echo ""
