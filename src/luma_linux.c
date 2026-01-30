#include <jni.h>
#include <android/log.h>
#include <stdbool.h>
#include <dlfcn.h>
#include "luma_module_manager.hpp" // Make sure this header contains the extern "C" wrappers

#define LOG_TAG "LUMA_LINUX"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// --- Hook Management ---
// These weak attributes allow the launcher to provide the hooking functions
__attribute__((weak)) void* mcpelauncher_hook(void* sym, void* hook, void** orig);

// Function pointers for the original game functions
void (*Keyboard_feed_orig)(int key, int action, int method);
void (*ScreenContext_render_orig)(void* ctx);

// --- 1. Keyboard Hook ('K' Key Toggle) ---
void Keyboard_feed_hook(int key, int action, int method) {
    // 0x4B is common for 'K'. Action 1 = KeyDown
    if (key == 0x4B && action == 1) {
        toggle_luma_menu(); // Calls C++ Manager
    }
    
    // Always call original so the game still works
    Keyboard_feed_orig(key, action, method);
}

// --- 2. Render Hook (Run Module Ticks) ---
void ScreenContext_render_hook(void* ctx) {
    ScreenContext_render_orig(ctx);
    
    // This is where Keystrokes/CPS Counter logic runs every frame
    run_luma_tick(); // Calls C++ Manager
}

// --- 3. Mod Initialization ---
void mod_preinit() {
    LOGI("======= LUMA CLIENT PRE-INIT =======");
    
    // Initialize the C++ Module Manager
    init_luma_manager();

    // Hooking the game symbols
    // Note: Symbols like "Keyboard::feed" must be found in the game's .so
    void* k_feed = dlsym(RTLD_DEFAULT, "_ZN8Keyboard4feedEiii"); // Example mangled name
    if (k_feed && mcpelauncher_hook) {
        mcpelauncher_hook(k_feed, (void*)Keyboard_feed_hook, (void**)&Keyboard_feed_orig);
        LOGI("Keyboard Hooked!");
    }

    void* s_render = dlsym(RTLD_DEFAULT, "_ZN13ScreenContext6renderEv"); // Example mangled name
    if (s_render && mcpelauncher_hook) {
        mcpelauncher_hook(s_render, (void*)ScreenContext_render_hook, (void**)&ScreenContext_render_orig);
        LOGI("Render Hooked!");
    }
}

__attribute__((constructor))
void luma_main_load() {
    LOGI("Luma Client v1.0.1 Loaded Successfully.");
}
