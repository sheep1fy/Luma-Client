#include "../../include/luma_module.h"
#include "../../include/luma_game_api.h"
#include "imgui.h"

static bool g_toggleSprint = false;

static auto LumaAutoSprintTick = [](float) {
    bool moving = LumaIsMovingForward();
    bool ground = LumaIsOnGround();
    
    if (!g_toggleSprint) {
        // Auto sprint when moving forward on ground
        LumaSetSprintKeyDown(moving && ground);
    } else {
        // Toggle sprint (Ctrl key)
        ImGuiIO& io = ImGui::GetIO();
        static bool prev = false;
        bool pressed = io.KeysDown[ImGuiKey_LeftCtrl];
        if (pressed && !prev) g_toggleSprint = !g_toggleSprint;
        prev = pressed;
        LumaSetSprintKeyDown(g_toggleSprint);
    }
};

static auto LumaAutoSprintRender = []() {
    if (ImGui::CollapsingHeader("AutoSprint")) {
        ImGui::Checkbox("Toggle Mode (Ctrl)", &g_toggleSprint);
    }
};

LumaModule LumaCreateAutoSprintModule() {
    return {"AutoSprint", true, LumaAutoSprintTick, LumaAutoSprintRender};
}
