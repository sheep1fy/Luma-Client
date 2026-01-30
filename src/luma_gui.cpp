// src/luma_gui.cpp
#include "luma_gui.h"
#include "luma_module_manager.h"
#include "imgui.h"

bool LumaHudEditMode = false;

static bool g_lumaGuiOpen = false;

static void LumaHandleGuiToggle() {
    ImGuiIO &io = ImGui::GetIO();
    static bool prev = false;
    bool cur = io.KeysDown[ImGuiKey_RightShift];
    if (cur && !prev)
        g_lumaGuiOpen = !g_lumaGuiOpen;
    prev = cur;
}

void LumaGUIRender() {
    LumaHandleGuiToggle();
    if (!g_lumaGuiOpen) return;

    ImGui::Begin("Luma Client");

    auto &mods = LumaGetModuleManager().getModules();
    for (auto &m : mods) {
        bool enabled = m.enabled;
        if (ImGui::Checkbox(m.name.c_str(), &enabled))
            m.enabled = enabled;
    }

    ImGui::Separator();
    ImGui::Checkbox("HUD editor", &LumaHudEditMode);

    // TODO: Add config load/save buttons here.

    ImGui::End();
}
