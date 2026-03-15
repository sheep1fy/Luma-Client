/*
 * luma_zoom_module.cpp
 * FOV zoom toggle (hold C to zoom).
 * Key state tracked via SDL hook.
 */

#include <luma_gui.h>
#include "imgui.h"
#include "../luma_camera_api.h"
#include <cstdio>
#include <cmath>

static bool  g_zoom_held   = false;
static float g_zoom_factor = 4.0f;     /* divide FOV by this */
static float g_current     = 1.0f;     /* smooth lerp target */

/* SDL hook calls this */
extern "C" void luma_zoom_on_key(int sdl_sym, bool pressed) {
    if (sdl_sym == SDLK_c) {
        g_zoom_held = pressed;
    }
}

static void update_zoom() {
    float target = g_zoom_held ? (1.0f / g_zoom_factor) : 1.0f;
    /* Simple lerp */
    g_current += (target - g_current) * 0.18f;
    if (fabsf(g_current - target) < 0.001f) g_current = target;
    luma_camera_set_fov_scale(g_current);
}

static LumaModule g_zoom_module = {
    .name    = "Zoom",
    .enabled = true,
    .x       = 10.0f,
    .y       = 365.0f,

    .renderHud = []() {
        update_zoom();

        if (!g_zoom_held) return;

        ImGui::SetNextWindowBgAlpha(0.40f);
        ImGui::SetNextWindowPos(ImVec2(g_zoom_module.x, g_zoom_module.y),
                                ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(130, 36), ImGuiCond_Always);

        ImGui::Begin("##zoom_hud", nullptr,
                     ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoInputs     |
                     ImGuiWindowFlags_NoMove        |
                     ImGuiWindowFlags_NoSavedSettings);

        ImGui::TextColored(ImVec4(1.0f,0.9f,0.4f,1.0f),
                           "ZOOM  %.1fx", g_zoom_factor);
        ImGui::End();
    },

    .renderSettings = []() {
        ImGui::SliderFloat("Zoom factor", &g_zoom_factor, 1.5f, 10.0f, "%.1fx");
        ImGui::TextDisabled("Hold C to zoom.");
    }
};

void luma_zoom_module_register() {
    LumaRegisterModule(&g_zoom_module);
}
