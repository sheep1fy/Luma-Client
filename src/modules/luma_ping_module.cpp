/*
 * luma_ping_module.cpp
 * Server latency display.
 */

#include <luma_gui.h>
#include "imgui.h"
#include "../luma_game_api.h"
#include <cstdio>

static LumaModule g_ping_module = {
    .name    = "Ping",
    .enabled = true,
    .x       = 10.0f,
    .y       = 320.0f,

    .renderHud = []() {
        int ping = luma_game_get_ping();

        ImGui::SetNextWindowBgAlpha(0.55f);
        ImGui::SetNextWindowPos(ImVec2(g_ping_module.x, g_ping_module.y),
                                ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(120, 36), ImGuiCond_Always);

        ImGui::Begin("##ping_hud", nullptr,
                     ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoInputs     |
                     ImGuiWindowFlags_NoMove        |
                     ImGuiWindowFlags_NoSavedSettings);

        ImVec4 col = ping < 60  ? ImVec4(0.4f, 1.0f, 0.4f, 1.0f)
                   : ping < 120 ? ImVec4(1.0f, 1.0f, 0.4f, 1.0f)
                                : ImVec4(1.0f, 0.4f, 0.4f, 1.0f);

        ImGui::TextColored(ImVec4(0.75f,0.45f,1.0f,1.0f), "Ping");
        ImGui::SameLine();
        ImGui::TextColored(col, "%d ms", ping);
        ImGui::End();
    },

    .renderSettings = nullptr
};

void luma_ping_module_register() {
    LumaRegisterModule(&g_ping_module);
}
