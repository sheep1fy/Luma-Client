// ============================================================================
// luma_linux.c - Linux / Android OpenGL hook + ImGui keybind
// ============================================================================

#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>

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
#endif

/* =========================
   C ↔ C++ Bridges
   ========================= */

#ifdef __cplusplus
extern "C" {
#endif

void luma_gui_render(void);
void luma_core_init(void);
bool luma_imgui_is_k_pressed(void);

#ifdef __cplusplus
}
#endif

/* =========================
   Globals
   ========================= */

static bool ui_open = false;
static bool last_k_state = false;

static void (*orig_glClear)(GLbitfield mask) = NULL;

/* =========================
   Key Handling (ImGui)
   ========================= */

static bool is_key_pressed_k() {
    return luma_imgui_is_k_pressed();
}

/* =========================
   Hooked glClear
   ========================= */

void glClear(GLbitfield mask) {
    if (!orig_glClear) {
        orig_glClear = dlsym(RTLD_NEXT, "glClear");
        if (!orig_glClear) return;
    }

    bool k_pressed = is_key_pressed_k();
    if (k_pressed && !last_k_state) {
        ui_open = !ui_open;
    }
    last_k_state = k_pressed;

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
