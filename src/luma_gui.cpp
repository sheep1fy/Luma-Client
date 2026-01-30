#include "luma_gui.h"
#include "luma_module_manager.h"
#include "luma_config.h"
#include "imgui.h"

bool LumaHudEditMode = false;

static bool g_guiOpen = false;

static void LumaToggleGUI() {
    ImGuiIO& io = ImGui::GetIO();
    static bool prevShift = false;
    bool shift = io.KeysDownRaw[ImGuiKey_RightShift];
    if (shift && !prevShift) g_guiOpen = !g_guiOpen;
    prevShift = shift;
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
