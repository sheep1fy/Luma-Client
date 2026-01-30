#include <vector>
#include <string>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "LUMA", __VA_ARGS__)

class Module {
public:
    std::string name;
    bool enabled = false;
    virtual void onTick() = 0; // Logic that runs every frame
};

// --- Legit Module: ToggleSprint ---
class ToggleSprint : public Module {
public:
    ToggleSprint() { name = "ToggleSprint"; }
    void onTick() override {
        if (enabled) {
            // Placeholder: Access GameInstance->LocalPlayer and set isSprinting
        }
    }
};

// --- Module Manager ---
class ModuleManager {
public:
    std::vector<Module*> modules;
    bool menuOpen = false;

    void init() {
        modules.push_back(new ToggleSprint());
        // Add Keystrokes, CPS Counter, etc. here
    }

    void toggleMenu() {
        menuOpen = !menuOpen;
        LOGI("Menu status: %s", menuOpen ? "Visible" : "Hidden");
    }

    void runModules() {
        for (auto mod : modules) {
            if (mod->enabled) mod->onTick();
        }
    }
};

ModuleManager g_ModuleManager;
