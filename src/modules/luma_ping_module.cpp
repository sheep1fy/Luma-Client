#include "../../include/luma_module.h"
#include "../../include/luma_hud.h"
#include "../../include/luma_game_api.h"
#include "imgui.h"
#include <cstdio>

static int g_ping = 0;

static auto LumaPingTick = [](float) {
    g_ping = LumaGetPingMs();
};

static auto LumaPingRender = []() {
    char buf[32];
    snprintf(buf, sizeof(buf), "%d ms", g_ping);
    
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    ImVec2 pos = g_lumaPingHud.pos;
    dl->AddText(pos + ImVec2(1,1), IM_COL32(0,0,0,200), buf);
    dl->AddText(pos, IM_COL32(255,255,0,255), buf);
};

LumaModule LumaCreatePingModule() {
    return {"Ping", true, LumaPingTick, LumaPingRender};
}
