// src/luma_core.cpp
#include "luma_module_manager.h"
#include "luma_gui.h"
#include "luma_hud.h"

// module creators
LumaModule LumaCreateFPSModule();
LumaModule LumaCreateCPSModule();
LumaModule LumaCreateKeystrokesModule();
LumaModule LumaCreateCoordsModule();
LumaModule LumaCreatePingModule();
LumaModule LumaCreateAutoSprintModule();
LumaModule LumaCreateZoomModule();

static bool g_lumaModulesRegistered = false;

static void LumaRegisterModules() {
    if (g_lumaModulesRegistered) return;
    g_lumaModulesRegistered = true;

    auto &mgr = LumaGetModuleManager();
    mgr.add(LumaCreateFPSModule());
    mgr.add(LumaCreateCPSModule());
    mgr.add(LumaCreateKeystrokesModule());
    mgr.add(LumaCreateCoordsModule());
    mgr.add(LumaCreatePingModule());
    mgr.add(LumaCreateAutoSprintModule());
    mgr.add(LumaCreateZoomModule());
}

void LumaOnTick(float dt) {
    LumaRegisterModules();
    LumaGetModuleManager().tick(dt);
}

void LumaOnRender() {
    LumaGUIRender();
    LumaHudEditorRender();
    LumaGetModuleManager().render();
}
