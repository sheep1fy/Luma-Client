// include/luma_module.h
#pragma once
#include <string>
#include <functional>

struct LumaModule {
    std::string name;
    bool enabled = true;
    std::function<void(float)> on_tick; // dt
    std::function<void()> on_render;    // ImGui + overlay
};
