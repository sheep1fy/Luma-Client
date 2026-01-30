#include "luma_module.h"
#include "luma_hud.h"
#include "imgui.h"
#include <deque>
#include <chrono>

using Clock = std::chrono::steady_clock;
struct LumaClickEvent { bool left; Clock::time_point t; };

static std::deque<LumaClickEvent> g_lumaClicks;

void LumaRegisterClick(bool left); // already declared, but we need definition
void LumaInternalRegisterClick(bool left) {
    g_lumaClicks.push_back({left, Clock::now()});
}

static void LumaCpsTick(float) {
    auto now = Clock::now();
    while (!g_lumaClicks.empty()) {
        float age = std::chrono::duration<float>(now - g_lumaClicks.front().t).count();
        if (age > 1.f) g_lumaClicks.pop_front();
        else break;
    }
}

static void LumaCpsRender() {
    int left = 0, right = 0;
    for (auto &c : g_lumaClicks) {
        if (c.left) left++; else right++;
    }
    char buf[64];
    snprintf(buf, sizeof(buf), "CPS L:%d R:%d", left, right);

    ImDrawList *dl = ImGui::GetBackgroundDrawList();
    ImVec2 pos = g_lumaCpsHud.pos;
    dl->AddText(pos + ImVec2(1,1), IM_COL32(0,0,0,200), buf);
    dl->AddText(pos, IM_COL32(255,255,255,255), buf);
}

LumaModule LumaCreateCPSModule() {
    LumaModule m;
    m.name = "CPS";
    m.enabled = true;
    m.on_tick = LumaCpsTick;
    m.on_render = LumaCpsRender;
    return m;
}
