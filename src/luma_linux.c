// ============================================================================
// luma_linux.c - Entry Point & Dobby Hook Implementations (GLES3 Fixed)
// ============================================================================
#include <jni.h>
#include <android/log.h>
#include <stdbool.h>
#include <stdint.h>
#include <dlfcn.h>
#include <GLES3/gl3.h>
#include "luma_module_manager.hpp"
// ============================================================================
// LOGGING MACROS
// ============================================================================
#define LOG_TAG "LUMA_CLIENT"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// ============================================================================
// DOBBY HOOKING LIBRARY
// ============================================================================
extern int DobbyHook(void* target, void* replace, void** result);

// ============================================================================
// ORIGINAL FUNCTION POINTERS
// ============================================================================
static void (*Keyboard_feed_original)(int key, int action, int method) = NULL;
static void (*ScreenContext_render_original)(void* screen_context) = NULL;

// ============================================================================
// MENU STATE
// ============================================================================
extern bool menu_open;

// ============================================================================
// HOOK 1: Keyboard::feed - Input Interception
// ============================================================================
static void Keyboard_feed_hook(int key, int action, int method) {
    if (key == 0x4B && action == 1) {
        LOGI("Menu toggle key pressed");
        toggle_luma_menu();
    }
    
    if (Keyboard_feed_original) {
        Keyboard_feed_original(key, action, method);
    }
}

// ============================================================================
// SIMPLIFIED GLES3 FULLSCREEN QUAD (No shaders needed)
// ============================================================================
// Pre-defined vertices for fullscreen quad in NDC (-1 to 1)
static const GLfloat fullscreen_quad[] = {
    -1.0f, -1.0f,  // bottom-left
     1.0f, -1.0f,  // bottom-right
    -1.0f,  1.0f,  // top-left
     1.0f,  1.0f   // top-right
};

// ============================================================================
// HOOK 2: ScreenContext::render - Render Loop (GLES3 Fixed)
// ============================================================================
static void ScreenContext_render_hook(void* screen_context) {
    // STEP 1: Render original game content
    if (ScreenContext_render_original) {
        ScreenContext_render_original(screen_context);
    }
    
    // STEP 2: Glassmorphism overlay (GLES3 compatible)
    if (menu_open) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        
        // Bind vertex attribute (attribute 0 = position)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, fullscreen_quad);
        
        // Draw fullscreen quad (black 40% alpha)
        glClearColor(0.0f, 0.0f, 0.0f, 0.4f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        // Cleanup
        glDisableVertexAttribArray(0);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
    
    // STEP 3: Render ImGui UI
    run_luma_tick();
}

// ============================================================================
// HOOK INSTALLATION
// ============================================================================
static void install_hooks() {
    LOGI("========================================");
    LOGI("  LUMA CLIENT - Installing Hooks");
    LOGI("========================================");
    
    void* keyboard_feed_addr = dlsym(RTLD_DEFAULT, "_ZN8Keyboard4feedEiii");
    if (keyboard_feed_addr) {
        int result = DobbyHook(keyboard_feed_addr, (void*)Keyboard_feed_hook, (void**)&Keyboard_feed_original);
        if (result == 0) {
            LOGI("✓ Keyboard::feed hooked successfully");
        } else {
            LOGE("✗ Failed to hook Keyboard::feed (error: %d)", result);
        }
    } else {
        LOGW("✗ Keyboard::feed symbol not found");
    }
    
    void* screen_render_addr = dlsym(RTLD_DEFAULT, "_ZN13ScreenContext6renderEv");
    if (screen_render_addr) {
        int result = DobbyHook(screen_render_addr, (void*)ScreenContext_render_hook, (void**)&ScreenContext_render_original);
        if (result == 0) {
            LOGI("✓ ScreenContext::render hooked successfully");
        } else {
            LOGE("✗ Failed to hook ScreenContext::render (error: %d)", result);
        }
    } else {
        LOGW("✗ ScreenContext::render symbol not found");
    }
    
    LOGI("========================================");
    LOGI("  Hook Installation Complete");
    LOGI("========================================");
}

// ============================================================================
// MOD ENTRY POINT
// ============================================================================
__attribute__((constructor))
static void luma_client_init() {
    LOGI("========================================");
    LOGI("  LUMA CLIENT v1.0.1");
    LOGI("  Platform: mcpelauncher (Linux x86_64)");
    LOGI("========================================");
    
    init_luma_manager();
    LOGI("✓ Module manager initialized");
    
    install_hooks();
    
    LOGI("========================================");
    LOGI("  Luma Client Loaded Successfully!");
    LOGI("  Press 'K' in-game to open the menu");
    LOGI("========================================");
}

__attribute__((destructor))
static void luma_client_cleanup() {
    LOGI("Luma Client unloading...");
}
