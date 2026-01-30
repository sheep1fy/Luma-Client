#include <GLES3/gl3.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"

// --- Module Structure ---
struct Module {
    const char* name;
    bool enabled;
    void (*onUpdate)(); // Logic run every frame
};

// --- Legit Feature: ToggleSprint ---
void updateToggleSprint() {
    // Logic: If moving, set the game's internal 'isSprinting' flag to true
}

Module modules[] = {
    {"ToggleSprint", false, updateToggleSprint},
    {"Keystrokes", true, nullptr},
    {"CPS Counter", true, nullptr}
};

// --- The 'K' Key UI ---
void RenderMenu() {
    ImGui::Begin("Luma PvP Client", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Press 'K' to hide/show");
    ImGui::Separator();

    for (int i = 0; i < 3; i++) {
        ImGui::Checkbox(modules[i].name, &modules[i].enabled);
    }
    ImGui::End();
}
