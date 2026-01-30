#include "luma_gui.h"
#include "luma_module_manager.h"
#include "luma_config.h"
#include "imgui.h"

bool LumaHudEditMode = false;

static bool g_guiOpen = false;

void LumaToggleGUI() {
    ImGuiIO& io = ImGui::GetIO();
    
    // Check if Right Shift + F3 is pressed
    bool shift = ImGui::IsKeyDown(ImGuiKey_RightShift);
    bool f3 = ImGui::IsKeyPressed(ImGuiKey_F3, false);
    
    if (shift && f3) {
        showLumaGUI = !showLumaGUI;
    }
}

void LumaGUIRender() {
    LumaToggleGUI();
    if (!g_guiOpen) return;

    ImGui::Begin("Luma Client v1.21.30");
    
    auto& mods = LumaGetModuleManager().getModules();
    for (auto& m : mods) {
        bool enabled = m.enabled;
        if (ImGui::Checkbox(m.name.c_str(), &enabled))
            m.enabled = enabled;
    }
    
    ImGui::Separator();
    ImGui::Checkbox("HUD Editor", &LumaHudEditMode);
    ImGui::Separator();
    if (ImGui::Button("Save Config")) LumaConfigSave();
    ImGui::SameLine();
    if (ImGui::Button("Load Config")) LumaConfigLoad();
    
    ImGui::End();
}
