#include "luma_gui.h"
#include "luma_module_manager.h"
#include "luma_config.h"
#include "imgui.h"

/* =========================
   GUI State
   ========================= */

static bool showLumaGUI = false;

/* =========================
   ImGui Key Bridge (C-callable)
   ========================= */

extern "C" bool luma_imgui_is_k_pressed()
{
    // Trigger once per press (no repeat spam)
    return ImGui::IsKeyPressed(ImGuiKey_K, false);
}

/* =========================
   GUI Toggle Logic
   ========================= */

static void LumaToggleGUI()
{
    if (luma_imgui_is_k_pressed()) {
        showLumaGUI = !showLumaGUI;
    }
}

/* =========================
   GUI Render
   ========================= */

void LumaGUIRender()
{
    LumaToggleGUI();
    if (!showLumaGUI) return;

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
