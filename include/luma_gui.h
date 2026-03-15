#pragma once

#ifdef __cplusplus
#include <string>
#include <vector>
#include <functional>

// ============================================================
// C++ API
// ============================================================

void LumaGUIRender();
void LumaGUIInit();
void LumaGUIShutdown();

extern bool LumaHudEditMode;
extern bool LumaMenuOpen;

struct LumaModule {
    std::string name;
    bool enabled;
    float x, y;
    std::function<void()> renderHud;
    std::function<void()> renderSettings;
};

void LumaRegisterModule(LumaModule* mod);
std::vector<LumaModule*>& LumaGetModules();

extern "C" {
#endif

// ============================================================
// C API (callable from C code)
// ============================================================

void luma_gui_render(void);
void luma_gui_init(void);
void luma_gui_shutdown(void);
int  luma_gui_is_open(void);
void luma_gui_toggle(void);

#ifdef __cplusplus
}
#endif
