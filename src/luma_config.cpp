/*
 * luma_config.cpp
 * Simple JSON-like config persistence.
 * Saves module enabled states and HUD positions to ~/.config/luma_client.cfg
 */

#include "luma_config.h"
#include <luma_gui.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

static char g_config_path[512] = {0};

static void build_path() {
    if (g_config_path[0]) return;
    const char *home = getenv("HOME");
    if (!home) home = "/tmp";
    snprintf(g_config_path, sizeof(g_config_path),
             "%s/.config/luma_client.cfg", home);
}

void luma_config_save() {
    build_path();
    FILE *f = fopen(g_config_path, "w");
    if (!f) {
        fprintf(stderr, "[LumaClient] Failed to open config for write: %s\n",
                g_config_path);
        return;
    }

    auto &modules = LumaGetModules();
    for (auto *mod : modules) {
        fprintf(f, "module.%s.enabled=%d\n", mod->name.c_str(), (int)mod->enabled);
        fprintf(f, "module.%s.x=%.1f\n",     mod->name.c_str(), mod->x);
        fprintf(f, "module.%s.y=%.1f\n",     mod->name.c_str(), mod->y);
    }

    fclose(f);
    fprintf(stderr, "[LumaClient] Config saved to %s\n", g_config_path);
}

void luma_config_load() {
    build_path();
    FILE *f = fopen(g_config_path, "r");
    if (!f) return; /* first run */

    char line[256];
    auto &modules = LumaGetModules();

    while (fgets(line, sizeof(line), f)) {
        /* strip newline */
        line[strcspn(line, "\r\n")] = 0;

        char key[128], val[128];
        if (sscanf(line, "%127[^=]=%127s", key, val) != 2) continue;

        for (auto *mod : modules) {
            char buf[128];

            snprintf(buf, sizeof(buf), "module.%s.enabled", mod->name.c_str());
            if (strcmp(key, buf) == 0) { mod->enabled = atoi(val) != 0; continue; }

            snprintf(buf, sizeof(buf), "module.%s.x", mod->name.c_str());
            if (strcmp(key, buf) == 0) { mod->x = (float)atof(val); continue; }

            snprintf(buf, sizeof(buf), "module.%s.y", mod->name.c_str());
            if (strcmp(key, buf) == 0) { mod->y = (float)atof(val); continue; }
        }
    }

    fclose(f);
    fprintf(stderr, "[LumaClient] Config loaded from %s\n", g_config_path);
}
