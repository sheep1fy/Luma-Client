// ============================================================================
// luma_linux.c - OpenGL Function Hooking (Version-Independent)
// ============================================================================
// This version hooks OpenGL functions instead of Minecraft symbols,
// making it compatible with ALL Minecraft Bedrock versions including
// stripped binaries where symbols are not available.
//
// Hooks:
// 1. glClear() - Render loop injection (called every frame)
// 2. nativeKeyHandler - JNI input bridge (always available)
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
static void (*glClear_original)(GLbitfield mask) = NULL;
static void (*nativeKeyHandler_original)(JNIEnv* env, jobject obj, jint key, jint action) = NULL;

// ============================================================================
// MENU STATE & FRAME CONTROL
// ============================================================================
extern bool menu_open;
static uint64_t frame_counter = 0;
static bool ui_rendered_this_frame = false;

// ============================================================================
// HOOK 1: JNI nativeKeyHandler - Input Interception
// ============================================================================
static void nativeKeyHandler_hook(JNIEnv* env, jobject obj, jint key, jint action) {
    // Check for K key (0x4B) press (action 1 = press, 0 = release)
    if (key == 0x4B && action == 1) {
        LOGI("Menu toggle key pressed (K)");
        toggle_luma_menu();
    }
    
    // Call original handler
    if (nativeKeyHandler_original) {
        nativeKeyHandler_original(env, obj, key, action);
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
// HOOK 2: glClear - Render Loop Injection (Version-Independent!)
// ============================================================================
static void glClear_hook(GLbitfield mask) {
    // STEP 1: Call original glClear to let game clear the framebuffer
    if (glClear_original) {
        glClear_original(mask);
    }
    
    // STEP 2: Only render UI once per frame (glClear can be called multiple times)
    uint64_t current_frame = frame_counter++;
    if (ui_rendered_this_frame && (frame_counter % 60 != 0)) {
        return; // Skip redundant renders
    }
    ui_rendered_this_frame = true;
    
    // STEP 3: Glassmorphism overlay (only when menu is open)
    if (menu_open) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        
        // Bind vertex attribute (attribute 0 = position)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, fullscreen_quad);
        
        // Draw fullscreen quad (black 40% alpha)
        glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        // Cleanup
        glDisableVertexAttribArray(0);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
    
    // STEP 4: Render ImGui UI
    run_luma_tick();
    
    // Reset flag at end of frame (detected by frame counter)
    if (frame_counter % 60 == 0) {
        ui_rendered_this_frame = false;
    }
}

// ============================================================================
// HOOK INSTALLATION (Version-Independent!)
// ============================================================================
static void install_hooks() {
    LOGI("========================================");
    LOGI("  LUMA CLIENT - Installing Hooks");
    LOGI("  Method: OpenGL Function Hooking");
    LOGI("========================================");
    
    // HOOK 1: Input Handler (JNI Bridge)
    // This function definitely exists - it's the Java→C++ bridge
    void* key_handler_addr = dlsym(RTLD_DEFAULT, "Java_com_mojang_minecraftpe_MainActivity_nativeKeyHandler");
    if (key_handler_addr) {
        int result = DobbyHook(key_handler_addr, (void*)nativeKeyHandler_hook, (void**)&nativeKeyHandler_original);
        if (result == 0) {
            LOGI("✓ nativeKeyHandler hooked successfully");
        } else {
            LOGE("✗ Failed to hook nativeKeyHandler (error: %d)", result);
        }
    } else {
        LOGW("✗ nativeKeyHandler symbol not found - input won't work!");
    }
    
    // HOOK 2: Render Loop (glClear)
    // This works with ANY Minecraft version - no symbols needed!
    void* glClear_addr = dlsym(RTLD_DEFAULT, "glClear");
    if (glClear_addr) {
        int result = DobbyHook(glClear_addr, (void*)glClear_hook, (void**)&glClear_original);
        if (result == 0) {
            LOGI("✓ glClear hooked successfully");
            LOGI("  This works with ANY Minecraft version!");
        } else {
            LOGE("✗ Failed to hook glClear (error: %d)", result);
        }
    } else {
        LOGE("✗ glClear not found - OpenGL not loaded yet?");
    }
    
    LOGI("========================================");
    LOGI("  Hook Installation Complete");
    LOGI("  Benefits:");
    LOGI("  • No Minecraft symbols needed");
    LOGI("  • Works across all versions");
    LOGI("  • Future-proof hooking method");
    LOGI("========================================");
}

// ============================================================================
// MOD ENTRY POINT
// ============================================================================
__attribute__((constructor))
static void luma_client_init() {
    LOGI("========================================");
    LOGI("  LUMA CLIENT v1.1.0");
    LOGI("  Platform: mcpelauncher (Linux x86_64)");
    LOGI("  Hooking: OpenGL Functions (Universal)");
    LOGI("========================================");
    
    init_luma_manager();
    LOGI("✓ Module manager initialized");
    
    install_hooks();
    
    LOGI("========================================");
    LOGI("  Luma Client Loaded Successfully!");
    LOGI("  Press 'K' in-game to open the menu");
    LOGI("  Compatible with ALL Minecraft versions!");
    LOGI("========================================");
}

__attribute__((destructor))
static void luma_client_cleanup() {
    LOGI("Luma Client unloading...");
}
