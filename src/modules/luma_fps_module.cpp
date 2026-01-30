#include "luma_module.h"
#include "luma_hud.h"
#include "imgui.h"

static float g_lumaFpsValue = 0.f;

static void LumaFpsTick(float dt) {
    if (dt > 0.f) {
        float inst = 1.f / dt;
        g_lumaFpsValue = 0.9f * g_lumaFpsValue + 0.1f * inst;
    }
}

static void LumaFpsRender() {
    ImDrawList *dl = ImGui::GetBackgroundDrawList();
    char buf[32];
    snprintf(buf, sizeof(buf), "FPS: %.0f", g_lumaFpsValue);
    ImVec2 pos = g_lumaFpsHud.pos;
    dl->AddText(pos + ImVec2(1,1), IM_COL32(0,0,0,200), buf);
    dl->AddText(pos, IM_COL32(255,255,255,255), buf);
}

LumaModule LumaCreateFPSModule() {
    LumaModule m;
    m.name = "FPS";
    m.enabled = true;
    m.on_tick = LumaFpsTick;
    m.on_render = LumaFpsRender;
    return m;
}
