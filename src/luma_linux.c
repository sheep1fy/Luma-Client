// ============================================================================
// luma_linux.c - Entry Point & Dobby Hook Implementations
// ============================================================================
// This file handles:
// 1. Library initialization (constructor)
// 2. Dobby hook setup for Keyboard::feed and ScreenContext::render
// 3. Menu toggle via 'K' key
// 4. Render overlay with glassmorphism blur effect
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
// DobbyHook is the primary function hooking mechanism
// Parameters:
//   target:  Original function address to hook
//   replace: Our replacement function
//   result:  Pointer to store the original function (for calling later)
// Returns: 0 on success, non-zero on failure
extern int DobbyHook(void* target, void* replace, void** result);

// ============================================================================
// ORIGINAL FUNCTION POINTERS
// ============================================================================
// These store the addresses of the original game functions so we can
// call them after our hook logic executes

// Keyboard::feed - Handles all keyboard input in the game
// Signature: void Keyboard::feed(int key, int action, int method)
static void (*Keyboard_feed_original)(int key, int action, int method) = NULL;

// ScreenContext::render - Main rendering loop for the game's UI
// Signature: void ScreenContext::render(ScreenContext* this)
static void (*ScreenContext_render_original)(void* screen_context) = NULL;

// ============================================================================
// MENU STATE (Shared with C++)
// ============================================================================
extern bool menu_open; // Defined in luma_module_manager.cpp

// ============================================================================
// HOOK 1: Keyboard::feed - Input Interception
// ============================================================================
// This hook intercepts all keyboard input before the game processes it.
// We use it to detect the 'K' key press to toggle the Luma menu.
//
// Minecraft Bedrock uses scan codes for keys:
//   0x4B = K key
//   action: 0 = key release, 1 = key press, 2 = key repeat
//   method: input method type (0 = keyboard, 1 = gamepad, etc.)
// ============================================================================
static void Keyboard_feed_hook(int key, int action, int method) {
    // Detect 'K' key press (scan code 0x4B, action 1 = press)
    if (key == 0x4B && action == 1) {
        LOGI("Menu toggle key pressed");
        toggle_luma_menu();
    }
    
    // IMPORTANT: Always call the original function to ensure the game
    // still receives all input events. Otherwise, the game won't respond
    // to any keyboard input!
    if (Keyboard_feed_original) {
        Keyboard_feed_original(key, action, method);
    }
}

// ============================================================================
// HOOK 2: ScreenContext::render - Render Loop Interception
// ============================================================================
// This hook is called every frame during the game's rendering phase.
// We use it to:
// 1. Render the original game content
// 2. Apply a glassmorphism overlay when the menu is open
// 3. Draw our custom ImGui interface on top
//
// The glassmorphism effect is achieved by rendering a semi-transparent
// black quad over the entire screen, creating a "blur" effect without
// expensive Gaussian blur computations.
// ============================================================================
static void ScreenContext_render_hook(void* screen_context) {
    // ========================================================================
    // STEP 1: Render the original game content
    // ========================================================================
    // This ensures the game's graphics are drawn first, and our UI
    // appears as an overlay on top
    if (ScreenContext_render_original) {
        ScreenContext_render_original(screen_context);
    }
    
    // ========================================================================
    // STEP 2: Apply glassmorphism overlay (when menu is open)
    // ========================================================================
    // We render a semi-transparent black rectangle over the entire viewport
    // to create a dimming/blur effect, making the menu stand out
    if (menu_open) {
        // Enable alpha blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Get viewport dimensions
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        GLint vp_width = viewport[2];
        GLint vp_height = viewport[3];
        
        // Disable depth testing (we want this to render on top)
        glDisable(GL_DEPTH_TEST);
        
        // Simple fullscreen quad using immediate mode (legacy but works)
        // Color: Black with 40% alpha (RGBA: 0, 0, 0, 0.4)
        // This creates the "glassmorphism" dimming effect
        
        // Note: For production, you'd use a proper shader and VBO,
        // but this works fine for a simple overlay
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrthof(0, vp_width, vp_height, 0, -1, 1);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        // Render the dimming quad
        glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
        glBegin(GL_QUADS);
            glVertex2f(0, 0);
            glVertex2f(vp_width, 0);
            glVertex2f(vp_width, vp_height);
            glVertex2f(0, vp_height);
        glEnd();
        
        // Restore matrices
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        
        // Re-enable depth testing
        glEnable(GL_DEPTH_TEST);
    }
    
    // ========================================================================
    // STEP 3: Render ImGui UI
    // ========================================================================
    // This calls into our C++ code (luma_module_manager.cpp) which handles
    // all the ImGui rendering logic for the modern UI
    run_luma_tick();
}

// ============================================================================
// HOOK INSTALLATION - Called During Mod Initialization
// ============================================================================
static void install_hooks() {
    LOGI("========================================");
    LOGI("  LUMA CLIENT - Installing Hooks");
    LOGI("========================================");
    
    // ========================================================================
    // Find Keyboard::feed symbol
    // ========================================================================
    // Symbol name: _ZN8Keyboard4feedEiii
    // This is the mangled C++ name for: Keyboard::feed(int, int, int)
    //
    // dlsym searches for this symbol in all loaded libraries (RTLD_DEFAULT)
    void* keyboard_feed_addr = dlsym(RTLD_DEFAULT, "_ZN8Keyboard4feedEiii");
    
    if (keyboard_feed_addr) {
        // Install the hook
        int result = DobbyHook(
            keyboard_feed_addr,              // Target function
            (void*)Keyboard_feed_hook,       // Our hook function
            (void**)&Keyboard_feed_original  // Store original here
        );
        
        if (result == 0) {
            LOGI("✓ Keyboard::feed hooked successfully");
        } else {
            LOGE("✗ Failed to hook Keyboard::feed (error: %d)", result);
        }
    } else {
        LOGW("✗ Keyboard::feed symbol not found");
        LOGW("  The game may have been updated with new symbol names");
        LOGW("  Try using: nm -D libminecraftpe.so | grep Keyboard");
    }
    
    // ========================================================================
    // Find ScreenContext::render symbol
    // ========================================================================
    // Symbol name: _ZN13ScreenContext6renderEv
    // This is the mangled C++ name for: ScreenContext::render(void)
    void* screen_render_addr = dlsym(RTLD_DEFAULT, "_ZN13ScreenContext6renderEv");
    
    if (screen_render_addr) {
        // Install the hook
        int result = DobbyHook(
            screen_render_addr,                   // Target function
            (void*)ScreenContext_render_hook,     // Our hook function
            (void**)&ScreenContext_render_original // Store original here
        );
        
        if (result == 0) {
            LOGI("✓ ScreenContext::render hooked successfully");
        } else {
            LOGE("✗ Failed to hook ScreenContext::render (error: %d)", result);
        }
    } else {
        LOGW("✗ ScreenContext::render symbol not found");
        LOGW("  The game may have been updated with new symbol names");
        LOGW("  Try using: nm -D libminecraftpe.so | grep ScreenContext");
    }
    
    LOGI("========================================");
    LOGI("  Hook Installation Complete");
    LOGI("========================================");
}

// ============================================================================
// MOD ENTRY POINT - Called When Library is Loaded
// ============================================================================
// The __attribute__((constructor)) tells the linker to call this function
// automatically when the shared library (.so) is loaded into memory.
//
// This happens early in the game startup process, before main() runs.
// ============================================================================
__attribute__((constructor))
static void luma_client_init() {
    LOGI("========================================");
    LOGI("  LUMA CLIENT v1.0.1");
    LOGI("  High-Performance Utility Mod");
    LOGI("  Platform: mcpelauncher (Linux x86_64)");
    LOGI("========================================");
    
    // Initialize the C++ module manager
    // This sets up ImGui and the module registry
    init_luma_manager();
    LOGI("✓ Module manager initialized");
    
    // Install all hooks
    install_hooks();
    
    LOGI("========================================");
    LOGI("  Luma Client Loaded Successfully!");
    LOGI("  Press 'K' in-game to open the menu");
    LOGI("========================================");
}

// ============================================================================
// OPTIONAL: Mod Cleanup on Unload
// ============================================================================
// This is called when the library is unloaded (rarely happens in practice)
__attribute__((destructor))
static void luma_client_cleanup() {
    LOGI("Luma Client unloading...");
    
    // TODO: Cleanup ImGui resources if needed
    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui::DestroyContext();
}
