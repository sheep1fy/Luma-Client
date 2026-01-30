#include "../../include/luma_module.h"
#include "../../include/luma_hud.h"
#include "imgui.h"

static auto LumaKeysRender = []() {
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 pos = g_lumaKeysHud.pos;
    ImVec2 sz = {28, 28};
    float pad = 4.f;
    
    auto drawKey = [&](ImVec2 p, const char* label, bool down) {
        ImU32 bg = down ? IM_COL32(70,140,255,255) : IM_COL32(40,40,40,200);
        dl->AddRectFilled(p, p+sz, bg, 4.f);
        dl->AddRect(p, p+sz, IM_COL32(0,0,0,255), 4.f);
        ImVec2 tsz = ImGui::CalcTextSize(label);
        dl->AddText(p + (sz-tsz)*0.5f, IM_COL32(255,255,255,255), label);
    };
    
    bool w = io.KeysDown[ImGuiKey_W];
    bool a = io.KeysDown[ImGuiKey_A];
    bool s = io.KeysDown[ImGuiKey_S];
    bool d = io.KeysDown[ImGuiKey_D];
    
    drawKey(pos, "W", w);
    drawKey(pos + ImVec2(0, sz.y+pad), "A", a);
    drawKey(pos + ImVec2(sz.x+pad, sz.y+pad), "S", s);
    drawKey(pos + ImVec2(2*(sz.x+pad), sz.y+pad), "D", d);
};

LumaModule LumaCreateKeystrokesModule() {
    return {"Keystrokes", true, nullptr, LumaKeysRender};
}
