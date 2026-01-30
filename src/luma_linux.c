#include <jni.h>
#include <android/log.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <GLES3/gl3.h>
#include "luma_module_manager.hpp"

#define LOG_TAG "LUMA_HOOKS"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// --- Hooking Setup ---
// Dobby is the standard library for function interception in Android/Linux mods
extern int DobbyHook(void* target, void* replace, void** result);

// Original function pointers
void (*Keyboard_feed_orig)(int key, int action, int method);
void (*ScreenContext_render_orig)(void* ctx);

// --- 1. Keyboard Hook (The 'K' Key) ---
void Keyboard_feed_hook(int key, int action, int method) {
    // 0x4B = Scan code for 'K'. Action 1 = Key Down
    if (key == 0x4B && action == 1) {
        toggle_luma_menu(); 
    }
    
    // Pass input to the original function so the game still receives it
    Keyboard_feed_orig(key, action, method);
}

// --- 2. Render Hook (The Blur & UI Layer) ---
void ScreenContext_render_hook(void* ctx) {
    // 1. Draw the actual game first
    ScreenContext_render_orig(ctx);

    // 2. Draw the "Blur" Overlay if menu is open
    // Since actual Gaussian blur is expensive, we use a 
    // smooth, semi-transparent black overlay to focus the eye.
    extern bool menu_open; // Accessed from C++ manager
    if (menu_open) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // This creates that "Sleek" dimmed background look
        // Color: R=0, G=0, B=0, A=0.4 (40% darkness)
        float dim_overlay[] = { 0.0f, 0.0f, 0.0f, 0.4f }; 
        glClearBufferfv(GL_COLOR, 0, dim_overlay);
    }

    // 3. Call the Modern C++ UI Tick
    run_luma_tick();
}

// --- 3. Mod Entry (Finding the Symbols) ---
void mod_preinit() {
    LOGI("======= LUMA CLIENT: ATTACHING HOOKS =======");
    
    // Initialize the module manager (C++ logic)
    init_luma_manager();

    // Finding Keyboard::feed (Input interception)
    void* k_feed = dlsym(RTLD_DEFAULT, "_ZN8Keyboard4feedEiii");
    if (k_feed) {
        DobbyHook(k_feed, (void*)Keyboard_feed_hook, (void**)&Keyboard_feed_orig);
        LOGI("Keyboard::feed hooked!");
    }

    // Finding ScreenContext::render (The Frame Loop)
    void* s_render = dlsym(RTLD_DEFAULT, "_ZN13ScreenContext6renderEv");
    if (s_render) {
        DobbyHook(s_render, (void*)ScreenContext_render_hook, (void**)&ScreenContext_render_orig);
        LOGI("ScreenContext::render hooked!");
    }
}

__attribute__((constructor))
void luma_loader() {
    LOGI("Luma PvP Client v1.0.1 (Flarial Style) successfully injected.");
}
