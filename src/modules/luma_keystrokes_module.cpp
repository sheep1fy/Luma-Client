/*
 * luma_keystrokes_module.cpp
 * WASD + mouse button visualizer.
 * Tracks key states via SDL event hook.
 */

#include <luma_gui.h>
#include "imgui.h"
#include <cstdio>

static bool g_w = false, g_a = false, g_s = false, g_d = false;
static bool g_lmb = false, g_rmb = false, g_space = false;

/* Called by SDL hook */
extern "C" void luma_keystrokes_on_key(int sdl_scancode, bool pressed) {
    switch (sdl_scancode) {
        case SDL_SCANCODE_W:     g_w     = pressed; break;
        case SDL_SCANCODE_A:     g_a     = pressed; break;
        case SDL_SCANCODE_S:     g_s     = pressed; break;
        case SDL_SCANCODE_D:     g_d     = pressed; break;
        case SDL_SCANCODE_SPACE: g_space = pressed; break;
    }
}

extern "C" void luma_keystrokes_on_mouse(int button, bool pressed) {
    if (button == 1) g_lmb = pressed;
    if (button == 3) g_rmb = pressed;
}

static void key_button(const char *label, bool pressed) {
    ImVec4 bg = pressed ? ImVec4(0.6f,0.3f,0.9f,1.0f) : ImVec4(0.2f,0.1f,0.3f,0.8f);
    ImGui::PushStyleColor(ImGuiCol_Button,        bg);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bg);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  bg);
    ImGui::Button(label, ImVec2(36, 30));
    ImGui::PopStyleColor(3);
}

static LumaModule g_ks_module = {
    .name    = "Keystrokes",
    .enabled = true,
    .x       = 10.0f,
    .y       = 200.0f,

    .renderHud = []() {
        ImGui::SetNextWindowBgAlpha(0.55f);
        ImGui::SetNextWindowPos(ImVec2(g_ks_module.x, g_ks_module.y),
                                ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(128, 110), ImGuiCond_Always);

        ImGui::Begin("##ks_hud", nullptr,
                     ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoInputs     |
                     ImGuiWindowFlags_NoMove        |
                     ImGuiWindowFlags_NoSavedSettings);

        /* Row 1 - W centered */
        ImGui::SetCursorPosX(46);
        key_button("W", g_w);

        /* Row 2 - A S D */
        key_button("A", g_a); ImGui::SameLine(0, 2);
        key_button("S", g_s); ImGui::SameLine(0, 2);
        key_button("D", g_d);

        /* Row 3 - LMB RMB */
        key_button("LMB", g_lmb); ImGui::SameLine(0, 2);
        key_button("RMB", g_rmb);

        ImGui::End();
    },

    .renderSettings = nullptr
};

void luma_keystrokes_module_register() {
    LumaRegisterModule(&g_ks_module);
}
