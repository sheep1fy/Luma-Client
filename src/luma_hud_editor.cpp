// src/luma_hud_editor.cpp
#include "luma_hud.h"
#include "imgui.h"

LumaHudElement g_lumaFpsHud  { "fps",   "FPS",       ImVec2(10,10),   ImVec2(60,20), 1.f };
LumaHudElement g_lumaCpsHud  { "cps",   "CPS",       ImVec2(10,30),   ImVec2(80,20), 1.f };
LumaHudElement g_lumaKeysHud { "keys",  "Keystrokes",ImVec2(10,60),   ImVec2(120,80),1.f };
LumaHudElement g_lumaCoordsHud { "coords","Coords",  ImVec2(10,150),  ImVec2(140,20),1.f };
LumaHudElement g_lumaPingHud { "ping",  "Ping",      ImVec2(10,170),  ImVec2(80,20), 1.f };

bool LumaHudEditMode = false;

static void LumaEditElement(LumaHudElement &e) {
    if (!LumaHudEditMode) return;

    ImDrawList *dl = ImGui::GetBackgroundDrawList();
    ImVec2 min = e.pos;
    ImVec2 max = e.pos + e.size;

    dl->AddRectFilled(min, max, IM_COL32(0, 0, 0, 80));
    dl->AddRect(min, max, IM_COL32(255,255,255,200));

    ImGui::SetCursorScreenPos(min);
    ImGui::InvisibleButton(e.id.c_str(), e.size);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        e.pos.x += delta.x;
        e.pos.y += delta.y;
    }

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("%s", e.label.c_str());
}

void LumaHudEditorRender() {
    LumaEditElement(g_lumaFpsHud);
    LumaEditElement(g_lumaCpsHud);
    LumaEditElement(g_lumaKeysHud);
    LumaEditElement(g_lumaCoordsHud);
    LumaEditElement(g_lumaPingHud);
}
