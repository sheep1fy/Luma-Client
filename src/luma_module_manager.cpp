#include "luma_module_manager.hpp"
#include <vector>
#include <android/log.h>

struct Module {
    const char* name;
    bool enabled;
};

std::vector<Module> modules;
bool menu_open = false;

extern "C" void init_luma_manager() {
    modules.push_back({"ToggleSprint", false});
    modules.push_back({"CPS Counter", true});
    modules.push_back({"Keystrokes", true});
}

extern "C" void toggle_luma_menu() {
    menu_open = !menu_open;
    __android_log_print(ANDROID_LOG_INFO, "LUMA", "Menu Toggled: %s", menu_open ? "ON" : "OFF");
}

extern "C" void run_luma_tick() {
    // This runs every frame. 
    // If ToggleSprint is enabled, we apply the sprint logic here.
}
