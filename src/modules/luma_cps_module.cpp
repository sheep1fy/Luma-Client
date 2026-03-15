/*
 * luma_cps_module.cpp
 * Clicks per second counter (hooks SDL mouse button events).
 */

#include <luma_gui.h>
#include "imgui.h"
#include <SDL2/SDL.h>
#include <time.h>
#include <cstdio>
#include <deque>

#define CPS_WINDOW_MS 1000  /* 1-second rolling window */

struct ClickRecord { long long time_ms; bool left; };

static std::deque<ClickRecord> g_clicks;

static long long now_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

/* Called from SDL hook in luma_linux.c via weak symbol (optional) */
extern "C" void luma_cps_on_click(bool left) {
    g_clicks.push_back({now_ms(), left});
}

static void prune_old() {
    long long threshold = now_ms() - CPS_WINDOW_MS;
    while (!g_clicks.empty() && g_clicks.front().time_ms < threshold)
        g_clicks.pop_front();
}

static LumaModule g_cps_module = {
    .name    = "CPS Counter",
    .enabled = true,
    .x       = 10.0f,
    .y       = 135.0f,

    .renderHud = []() {
        prune_old();
        int lcps = 0, rcps = 0;
        for (auto &c : g_clicks) {
            if (c.left) lcps++; else rcps++;
        }

        ImGui::SetNextWindowBgAlpha(0.55f);
        ImGui::SetNextWindowPos(ImVec2(g_cps_module.x, g_cps_module.y),
                                ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(140, 52), ImGuiCond_Always);

        ImGui::Begin("##cps_hud", nullptr,
                     ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoInputs     |
                     ImGuiWindowFlags_NoMove        |
                     ImGuiWindowFlags_NoSavedSettings);

        ImGui::TextColored(ImVec4(0.75f,0.45f,1.0f,1.0f), "CPS");
        ImGui::SameLine();
        ImGui::Text("L:%d  R:%d", lcps, rcps);
        ImGui::End();
    },

    .renderSettings = nullptr
};

void luma_cps_module_register() {
    LumaRegisterModule(&g_cps_module);
}
