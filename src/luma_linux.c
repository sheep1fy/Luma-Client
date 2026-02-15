// ============================================================================
// luma_linux.c - Linux / Android OpenGL hook + Direct X11 keybind
// ============================================================================

#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include "luma_gui.h"

/* =========================
   OpenGL / GLES Compatibility
   ========================= */

#ifdef ANDROID
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #ifndef glColor4f
        #define glColor4f(r, g, b, a) ((void)0)
    #endif
#else
    #include <GL/gl.h>
    // X11 for direct keyboard access on Linux
    #include <X11/Xlib.h>
    #include <X11/keysym.h>
#endif

/* =========================
   C ↔ C++ Bridges
   ========================= */

#ifdef __cplusplus
extern "C" {
#endif

void luma_gui_render(void);
void luma_core_init(void);

#ifdef __cplusplus
}
#endif

/* =========================
   Globals
   ========================= */

static bool ui_open = false;
static bool last_k_state = false;
static void (*orig_glClear)(GLbitfield mask) = NULL;

#ifndef ANDROID
static Display *x11_display = NULL;
#endif

/* =========================
   Direct X11 Key Detection (Linux)
   ========================= */

#ifndef ANDROID
static bool is_key_k_pressed_x11() {
    // Lazy init X11 connection
    if (!x11_display) {
        x11_display = XOpenDisplay(NULL);
        if (!x11_display) {
            return false;  // No X11, can't detect keys
        }
    }
    
    // Query keyboard state
    char keys[32];
    XQueryKeymap(x11_display, keys);
    
    // Get keycode for 'K' key
    KeyCode k_keycode = XKeysymToKeycode(x11_display, XK_k);
    
    // Check if key is pressed (bit array)
    return (keys[k_keycode / 8] & (1 << (k_keycode % 8))) != 0;
}
#else
// Android fallback (would need different input method)
static bool is_key_k_pressed_x11() {
    return false;
}
#endif

/* =========================
   Hooked glClear
   ========================= */

void glClear(GLbitfield mask) {
    if (!orig_glClear) {
        orig_glClear = dlsym(RTLD_NEXT, "glClear");
        if (!orig_glClear) return;
    }

    // Detect K key press (edge-triggered)
    bool k_pressed = is_key_k_pressed_x11();
    if (k_pressed && !last_k_state) {
        ui_open = !ui_open;
    }
    last_k_state = k_pressed;

    // Render GUI if open
    if (ui_open) {
        glColor4f(0.f, 0.f, 0.f, 0.4f);
        luma_gui_render();
    }

    orig_glClear(mask);
}

/* =========================
   Library Init
   ========================= */

__attribute__((constructor))
static void luma_init() {
    luma_core_init();
}

/* =========================
   Cleanup
   ========================= */

__attribute__((destructor))
static void luma_cleanup() {
#ifndef ANDROID
    if (x11_display) {
        XCloseDisplay(x11_display);
        x11_display = NULL;
    }
#endif
}
