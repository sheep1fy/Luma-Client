/*
 * luma_fps_module.cpp
 * FPS Counter HUD module.
 * Measures frame delta time via clock_gettime and smooths with a rolling buffer.
 */

#include <luma_gui.h>
#include "imgui.h"
#include <time.h>
#include <cstdio>
#include <cstring>
#include <cmath>

#define SAMPLE_COUNT 30

static float g_samples[SAMPLE_COUNT] = {};
static int   g_sample_idx   = 0;
static float g_last_fps     = 0.0f;
static struct timespec g_last_time = {};

static float compute_fps() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    double delta = (now.tv_sec  - g_last_time.tv_sec) +
                   (now.tv_nsec - g_last_time.tv_nsec) * 1e-9;
    g_last_time = now;

    if (delta <= 0.0) return g_last_fps;

    float fps = (float)(1.0 / delta);
    g_samples[g_sample_idx] = fps;
    g_sample_idx = (g_sample_idx + 1) % SAMPLE_COUNT;

    float sum = 0.0f;
    for (int i = 0; i < SAMPLE_COUNT; i++) sum += g_samples[i];
    g_last_fps = sum / SAMPLE_COUNT;
    return g_last_fps;
}

static LumaModule g_fps_module = {
    .name    = "FPS Counter",
    .enabled = true,
    .x       = 10.0f,
    .y       = 10.0f,

    .renderHud = []() {
        float fps = compute_fps();

        ImGui::SetNextWindowBgAlpha(0.55f);
        ImGui::SetNextWindowPos(ImVec2(g_fps_module.x, g_fps_module.y),
                                ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(120, 36), ImGuiCond_Always);

        ImGui::Begin("##fps_hud", nullptr,
                     ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoInputs     |
                     ImGuiWindowFlags_NoMove        |
                     ImGuiWindowFlags_NoSavedSettings);

        ImVec4 col = fps >= 60 ? ImVec4(0.4f, 1.0f, 0.4f, 1.0f)
                   : fps >= 30 ? ImVec4(1.0f, 1.0f, 0.4f, 1.0f)
                               : ImVec4(1.0f, 0.4f, 0.4f, 1.0f);

        ImGui::TextColored(col, "FPS  %.0f", fps);
        ImGui::End();
    },

    .renderSettings = []() {
        ImGui::TextDisabled("No settings available.");
    }
};

void luma_fps_module_register() {
    clock_gettime(CLOCK_MONOTONIC, &g_last_time);
    memset(g_samples, 0, sizeof(g_samples));
    LumaRegisterModule(&g_fps_module);
}
