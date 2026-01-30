#include "luma_config.h"
#include "luma_hud.h"
#include "luma_module_manager.h"
#include <fstream>
#include <string>

static const std::string CONFIG_PATH = "~/.local/share/mcpelauncher/luma_config.txt";

void LumaConfigSave() {
    std::ofstream file(CONFIG_PATH);
    if (!file) return;
    
    // Save HUD positions
    file << "fps_x=" << g_lumaFpsHud.pos.x << "\n";
    file << "fps_y=" << g_lumaFpsHud.pos.y << "\n";
    
    auto& mods = LumaGetModuleManager().getModules();
    for (auto& m : mods)
        file << "module_" << m.name << "=" << m.enabled << "\n";
}

void LumaConfigLoad() {
    std::ifstream file(CONFIG_PATH);
    if (!file) return;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("fps_x=") == 0) {
            g_lumaFpsHud.pos.x = std::stof(line.substr(6));
        } else if (line.find("fps_y=") == 0) {
            g_lumaFpsHud.pos.y = std::stof(line.substr(6));
        }
    }
}
