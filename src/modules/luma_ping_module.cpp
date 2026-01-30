#include "../../include/luma_module.h"
#include "../../include/luma_hud.h"
#include "../../include/luma_game_api.h"
#include "imgui.h"
#include <cstdio>

static LumaVec3 g_pos = {0, 64, 0};

static auto LumaCoordsTick = [](float) {
    g_pos = LumaGetPlayerPosition();
};

static auto LumaCoordsRender = []() {
    char buf[64];
    snprintf(buf, sizeof(buf), "X:%.1f Y:%.1f Z:%.1f", g_pos.x, g_pos.y, g_pos.z);
    
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    ImVec2 pos = g_lumaCoordsHud.pos;
    dl->AddText(pos + ImVec2(1,1), IM_COL32(0,0,0,200), buf);
    dl->AddText(pos, IM_COL32(0,255,0,255), buf);
};

LumaModule LumaCreateCoordsModule() {
    return {"Coords", true, LumaCoordsTick, LumaCoordsRender};
}
