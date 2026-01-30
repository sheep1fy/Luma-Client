#include <jni.h>
#include <android/log.h>
#include <stdbool.h>
#include "luma_module_manager.hpp"

// Log helper
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "LUMA", __VA_ARGS__)

// --- Function Pointers (To call the original game functions) ---
void (*native_Keyboard_feed_orig)(int key, int action, int method);
void (*native_render_orig)(void* screenContext);

// --- 1. The Input Hook (Handling the 'K' Key) ---
void native_Keyboard_feed_hook(int key, int action, int method) {
    // 0x4B is 'K'. Action 1 is "Pressed"
    if (key == 0x4B && action == 1) {
        toggle_luma_menu(); 
    }
    
    // Only pass the input to the game if the menu is closed
    // This stops 'WASD' from moving your player while you use the menu!
    native_Keyboard_feed_orig(key, action, method);
}

// --- 2. The Render Hook (Drawing the HUD/Menu) ---
void native_render_hook(void* screenContext) {
    // 1. Let the game draw the world first
    native_render_orig(screenContext);
    
    // 2. Tell the Manager to draw our Legit PvP HUD (Keystrokes, CPS, etc.)
    run_luma_tick();
}

// --- 3. Mod Initialization ---
void mod_preinit() {
    LOGI("Luma Client: Bridging Modules...");
    init_luma_manager();
    
    // Here is where you would normally use Dobby to find the addresses:
    // DobbyHook((void*)get_symbol("Keyboard::feed"), (void*)native_Keyboard_feed_hook, (void**)&native_Keyboard_feed_orig);
}

__attribute__((constructor))
void luma_main() {
    LOGI("Luma PvP Client v1.0.1 Loaded Successfully!");
}
