#include "luma_config.h"
#include "luma_hud.h"
#include "luma_module_manager.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

static const char* CONFIG_PATH = "~/.local/share/mcpelauncher/luma_config.json";

void LumaConfigSave() {
    json j;
    
    // Save HUD positions
    j["hud"]["fps"]["x"] = g_lumaFpsHud.pos.x;
    j["hud"]["fps"]["y"] = g_lumaFpsHud.pos.y;
    j["hud"]["cps"]["x"] = g_lumaCpsHud.pos.x;
    // ... more HUD elements
    
    // Save module states
    auto& mods = LumaGetModuleManager().getModules();
    for (auto& m : mods)
        j["modules"][m.name] = m.enabled;
    
    std::ofstream file(std::string(CONFIG_PATH));
    file << j.dump(2);
}

void LumaConfigLoad() {
    std::ifstream file(std::string(CONFIG_PATH));
    if (!file.good()) return;
    
    json j;
    file >> j;
    
    // Load HUD positions
    if (j.contains("hud")) {
        g_lumaFpsHud.pos.x = j["hud"]["fps"]["x"];
        g_lumaFpsHud.pos.y = j["hud"]["fps"]["y"];
        // ... more HUD elements
    }
}
