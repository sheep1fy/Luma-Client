/*
 * luma_hud_editor.cpp
 * Drag-and-drop HUD editor overlay.
 * When active, module windows become movable and their positions are saved.
 */

#include "luma_hud_editor.h"
#include <luma_gui.h>
#include "imgui.h"
#include <cstdio>

void LumaHudEditorRender() {
    /* Semi-transparent overlay hint */
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *draw = ImGui::GetBackgroundDrawList();

    draw->AddRectFilled(ImVec2(0, 0), io.DisplaySize,
                        IM_COL32(0, 0, 0, 60)); /* dim background */

    draw->AddText(ImVec2(10, 10),
                  IM_COL32(200, 150, 255, 220),
                  "HUD EDIT MODE  -  Drag modules to reposition  |  Toggle in menu");

    /* Each enabled module renders its HUD with ImGuiWindowFlags_NoTitleBar
     * but with movable windows when in edit mode — handled per module. */

    auto &modules = LumaGetModules();
    for (auto *mod : modules) {
        if (!mod->enabled) continue;

        /* Draw a coloured border around each module position */
        ImGui::SetNextWindowBgAlpha(0.4f);
        ImGui::SetNextWindowSize(ImVec2(160, 60), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(mod->x, mod->y), ImGuiCond_Once);

        int flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        char title[64];
        snprintf(title, sizeof(title), "%s##editor", mod->name.c_str());

        if (ImGui::Begin(title, nullptr, flags)) {
            ImVec2 pos = ImGui::GetWindowPos();
            mod->x = pos.x;
            mod->y = pos.y;
            ImGui::TextDisabled("%s", mod->name.c_str());
        }
        ImGui::End();
    }
}
