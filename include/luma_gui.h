// include/luma_gui.h
#pragma once

#ifdef __cplusplus
// C++ API
void LumaGUIRender();
extern bool LumaHudEditMode;

extern "C" {
#endif

// C API wrapper
void luma_gui_render(void);

#ifdef __cplusplus
}
#endif
