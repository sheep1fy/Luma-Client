// src/luma_module_manager.cpp
#include "luma_module_manager.h"

static LumaModuleManager g_lumaModules;

LumaModuleManager &LumaGetModuleManager() {
    return g_lumaModules;
}

void LumaModuleManager::tick(float dt) {
    for (auto &m : modules)
        if (m.enabled && m.on_tick)
            m.on_tick(dt);
}

void LumaModuleManager::render() {
    for (auto &m : modules)
        if (m.enabled && m.on_render)
            m.on_render();
}
