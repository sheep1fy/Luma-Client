/*
 * luma_core.cpp
 * Registers all built-in modules at startup.
 */

#include <luma_gui.h>
#include <cstdio>

/* Forward declarations for module init functions */
void luma_fps_module_register();
void luma_coords_module_register();
void luma_cps_module_register();
void luma_keystrokes_module_register();
void luma_ping_module_register();
void luma_zoom_module_register();
void luma_autosprint_module_register();

__attribute__((constructor))
static void luma_core_init() {
    fprintf(stderr, "[LumaClient] Registering modules...\n");

    luma_fps_module_register();
    luma_coords_module_register();
    luma_cps_module_register();
    luma_keystrokes_module_register();
    luma_ping_module_register();
    luma_zoom_module_register();
    luma_autosprint_module_register();

    fprintf(stderr, "[LumaClient] All modules registered\n");
}
