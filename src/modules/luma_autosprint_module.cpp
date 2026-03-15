/*
 * luma_autosprint_module.cpp
 * Automatically injects sprint input whenever the player moves forward.
 * Hooks SDL keyboard so we can inject a synthetic sprint key via game API.
 */

#include <luma_gui.h>
#include "imgui.h"
#include "../luma_game_api.h"
#include <cstdio>

static bool g_w_held = false;

extern "C" void luma_autosprint_on_key(int sdl_sym, bool pressed) {
    if (sdl_sym == SDLK_w) g_w_held = pressed;
}

static LumaModule g_sprint_module = {
    .name    = "Auto Sprint",
    .enabled = false, /* opt-in */
    .x       = 10.0f,
    .y       = 410.0f,

    .renderHud = []() {
        if (!g_sprint_module.enabled) return;

        /* When W is held, signal game API to sprint */
        luma_game_set_auto_sprint(g_w_held);

        if (!g_w_held) return;

        ImGui::SetNextWindowBgAlpha(0.40f);
        ImGui::SetNextWindowPos(ImVec2(g_sprint_module.x, g_sprint_module.y),
                                ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(130, 36), ImGuiCond_Always);

        ImGui::Begin("##sprint_hud", nullptr,
                     ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoInputs     |
                     ImGuiWindowFlags_NoMove        |
                     ImGuiWindowFlags_NoSavedSettings);

        ImGui::TextColored(ImVec4(0.4f,1.0f,0.6f,1.0f), "SPRINTING");
        ImGui::End();
    },

    .renderSettings = []() {
        ImGui::TextWrapped(
            "Automatically sprints when moving forward (W key). "
            "Requires game-side sprint hook for full effect.");
    }
};

void luma_autosprint_module_register() {
    LumaRegisterModule(&g_sprint_module);
}
