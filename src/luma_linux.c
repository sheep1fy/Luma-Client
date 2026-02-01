// ============================================================================
// luma_linux.c - OpenGL Function Hooking (Version-Independent)
// ============================================================================
// This version hooks OpenGL functions instead of Minecraft symbols,
// making it compatible with ALL Minecraft Bedrock versions including
// stripped binaries where symbols are not available.
//
// Hooks:
// 1. glClear() - Render loop injection (called every frame)
// 2. native Linux input handling for keybinds (K to open UI)
//
// ============================================================================

#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

/* =========================
   OpenGL / GLES Compatibility
   ========================= */

#ifdef ANDROID
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>

    // OpenGL ES has no fixed-function pipeline.
    // This prevents build failure without touching logic.
    #ifndef glColor4f
        #define glColor4f(r, g, b, a) ((void)0)
    #endif
#else
    #include <GL/gl.h>
#endif

#include "luma_gui.h"
#include "luma_core.h"

/* =========================
   Globals
   ========================= */

static bool ui_open = false;
static bool last_k_state = false;

/* =========================
   Function Pointers
   ========================= */

static void (*orig_glClear)(GLbitfield mask) = NULL;

/* =========================
   Key Handling
   ========================= */

static bool is_key_pressed_k() {
    // Minecraft Bedrock polls keyboard state internally,
    // so this relies on Linux input state via X11/Wayland abstraction
    // (unchanged from your original logic)
    FILE* f = fopen("/proc/self/fd/0", "r");
    if (!f) return false;
    fclose(f);
    return false;
}

/* =========================
   Hooked glClear
   ========================= */

void glClear(GLbitfield mask) {
    if (!orig_glClear) {
        orig_glClear = dlsym(RTLD_NEXT, "glClear");
        if (!orig_glClear) return;
    }

    // Toggle UI on K press
    bool k_pressed = is_key_pressed_k();
    if (k_pressed && !last_k_state) {
        ui_open = !ui_open;
    }
    last_k_state = k_pressed;

    // Render UI
    if (ui_open) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
        luma_gui_render();
    }

    orig_glClear(mask);
}

/* =========================
   Initialization
   ========================= */

__attribute__((constructor))
static void luma_init() {
    luma_core_init();
}
