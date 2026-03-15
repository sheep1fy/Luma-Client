/*
 * luma_module_manager.cpp
 * Central registry for all Luma modules.
 */

#include <luma_gui.h>
#include <vector>

static std::vector<LumaModule*> g_modules;

void LumaRegisterModule(LumaModule *mod) {
    if (mod) g_modules.push_back(mod);
}

std::vector<LumaModule*>& LumaGetModules() {
    return g_modules;
}
