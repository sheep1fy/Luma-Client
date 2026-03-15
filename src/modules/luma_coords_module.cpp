/*
 * luma_coords_module.cpp
 * XYZ coordinate display.
 */

#include <luma_gui.h>
#include "imgui.h"
#include "../luma_game_api.h"
#include <cstdio>

static LumaModule g_coords_module = {
    .name    = "Coordinates",
    .enabled = true,
    .x       = 10.0f,
    .y       = 55.0f,

    .renderHud = []() {
        ImGui::SetNextWindowBgAlpha(0.55f);
        ImGui::SetNextWindowPos(ImVec2(g_coords_module.x, g_coords_module.y),
                                ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(185, 70), ImGuiCond_Always);

        ImGui::Begin("##coords_hud", nullptr,
                     ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoInputs     |
                     ImGuiWindowFlags_NoMove        |
                     ImGuiWindowFlags_NoSavedSettings);

        ImGui::TextColored(ImVec4(0.75f,0.45f,1.0f,1.0f), "XYZ");
        ImGui::SameLine();
        ImGui::Text("%.2f  %.2f  %.2f",
                    luma_game_get_x(),
                    luma_game_get_y(),
                    luma_game_get_z());
        ImGui::End();
    },

    .renderSettings = []() {
        ImGui::TextDisabled("Coordinates are read from game state.");
    }
};

void luma_coords_module_register() {
    LumaRegisterModule(&g_coords_module);
}
