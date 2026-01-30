// include/luma_hud.h
#pragma once
#include "imgui.h"
#include <string>

struct LumaHudElement {
    std::string id;
    std::string label;
    ImVec2 pos {50.f, 50.f};
    ImVec2 size {100.f, 20.f};
    float scale = 1.f;
};

extern bool LumaHudEditMode;

// declare global HUD elements (definitions in a .cpp)
extern LumaHudElement g_lumaFpsHud;
extern LumaHudElement g_lumaCpsHud;
extern LumaHudElement g_lumaKeysHud;
extern LumaHudElement g_lumaCoordsHud;
extern LumaHudElement g_lumaPingHud;

void LumaHudEditorRender();
