// include/luma_module_manager.h
#pragma once
#include "luma_module.h"
#include <vector>

class LumaModuleManager {
public:
    void add(const LumaModule &m) { modules.push_back(m); }
    void tick(float dt);
    void render();
    std::vector<LumaModule> &getModules() { return modules; }

private:
    std::vector<LumaModule> modules;
};

LumaModuleManager &LumaGetModuleManager();
