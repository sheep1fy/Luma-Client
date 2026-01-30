#include "luma_module.h"
#include "luma_camera_api.h"
#include "imgui.h"
#include <cmath>

struct LumaZoomState {
    bool toggleMode = false;
    bool zooming = false;
    bool keyHeld = false;
    float defaultFov = 70.f;
    float zoomFov = 20.f;
    float smoothSpeed = 10.f;
};

static LumaZoomState g_lumaZoom;

static void LumaZoomTick(float dt) {
    ImGuiIO &io = ImGui::GetIO();
    bool key = io.KeysDown[ImGuiKey_LeftAlt] || io.MouseDown[1];

    if (g_lumaZoom.toggleMode) {
        if (key && !g_lumaZoom.keyHeld) {
            g_lumaZoom.zooming = !g_lumaZoom.zooming;
            if (g_lumaZoom.zooming)
                g_lumaZoom.defaultFov = LumaGetCurrentFOV();
            else
                LumaSetCurrentFOV(g_lumaZoom.defaultFov);
        }
        g_lumaZoom.keyHeld = key;
    } else {
        bool prev = g_lumaZoom.keyHeld;
        g_lumaZoom.keyHeld = key;
        if (g_lumaZoom.keyHeld && !prev) {
            g_lumaZoom.defaultFov = LumaGetCurrentFOV();
            g_lumaZoom.zooming = true;
        } else if (!g_lumaZoom.keyHeld && prev) {
            g_lumaZoom.zooming = false;
        }
    }

    float current = LumaGetCurrentFOV();
    float target = g_lumaZoom.zooming ? g_lumaZoom.zoomFov : g_lumaZoom.defaultFov;
    float lerp = 1.f - std::exp(-g_lumaZoom.smoothSpeed * dt);
    float nf = current + (target - current) * lerp;
    LumaSetCurrentFOV(nf);
}

static void LumaZoomRender() {
    if (ImGui::CollapsingHeader("Zoom")) {
        ImGui::Checkbox("Toggle mode", &g_lumaZoom.toggleMode);
        ImGui::SliderFloat("Zoom FOV", &g_lumaZoom.zoomFov, 5.f, 70.f);
        ImGui::SliderFloat("Smooth speed", &g_lumaZoom.smoothSpeed, 1.f, 30.f);
    }
}

LumaModule LumaCreateZoomModule() {
    LumaModule m;
    m.name = "Zoom";
    m.enabled = true;
    m.on_tick = LumaZoomTick;
    m.on_render = LumaZoomRender;
    return m;
}
