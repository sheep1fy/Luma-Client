/*
 * luma_linux.c
 * Luma Client v2.0.0 - Linux entry point
 *
 * Responsibilities:
 *   - Intercept glClear via RTLD_NEXT (version-independent rendering hook)
 *   - Intercept SDL_PollEvent / SDL_PeepEvents for keyboard input
 *   - Works on both Wayland and X11 sessions
 */

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* OpenGL ES */
#include <GLES2/gl2.h>

/* SDL2 event types */
#include <SDL2/SDL.h>

/* Luma GUI (C API) */
#include <luma_gui.h>

/* ─── Logging ─────────────────────────────────────────────────── */
#define LOG(fmt, ...) fprintf(stderr, "[LumaClient] " fmt "\n", ##__VA_ARGS__)

/* ─── State ───────────────────────────────────────────────────── */
static bool g_initialized    = false;
static bool g_sdl_hooked     = false;
static int  g_frame_counter  = 0;

/* Original function pointers */
static void (*orig_glClear)(GLbitfield mask)                              = NULL;
static int  (*orig_SDL_PollEvent)(SDL_Event *event)                       = NULL;
static int  (*orig_SDL_PeepEvents)(SDL_Event *events, int numevents,
                                   SDL_eventaction action,
                                   Uint32 minType, Uint32 maxType)        = NULL;

/* ─── One-time initialization ─────────────────────────────────── */
static void luma_init(void) {
    if (g_initialized) return;
    g_initialized = true;

    LOG("Luma Client v2.0.0 - Initializing");
    LOG("Wayland/X11 compatible input hooking active");

    luma_gui_init();

    LOG("Core initialization complete");
}

/* ─── SDL2 event hooks ────────────────────────────────────────── */

/*
 * Process a single SDL_Event.
 * Returns true if the event should be suppressed (not passed to game).
 * Currently we pass everything through - we only peek at input.
 */
static bool process_sdl_event(const SDL_Event *event) {
    if (!event) return false;

    switch (event->type) {
        case SDL_KEYDOWN:
            if (event->key.repeat == 0) {
                SDL_Keycode sym = event->key.keysym.sym;

                /* K = toggle menu */
                if (sym == SDLK_k) {
                    luma_gui_toggle();
                    LOG("K pressed - UI toggled to: %s",
                        luma_gui_is_open() ? "OPEN" : "CLOSED");
                }
            }
            break;

        case SDL_KEYUP:
            break;

        default:
            break;
    }
    return false; /* don't suppress */
}

/* Hook: SDL_PollEvent */
int SDL_PollEvent(SDL_Event *event) {
    if (!orig_SDL_PollEvent) {
        orig_SDL_PollEvent = dlsym(RTLD_NEXT, "SDL_PollEvent");
        if (orig_SDL_PollEvent && !g_sdl_hooked) {
            g_sdl_hooked = true;
            LOG("Hooked SDL_PollEvent successfully");
        }
    }

    int result = orig_SDL_PollEvent(event);

    if (result && event) {
        process_sdl_event(event);
    }

    return result;
}

/* Hook: SDL_PeepEvents (batch event processing) */
int SDL_PeepEvents(SDL_Event *events, int numevents,
                   SDL_eventaction action, Uint32 minType, Uint32 maxType) {
    if (!orig_SDL_PeepEvents) {
        orig_SDL_PeepEvents = dlsym(RTLD_NEXT, "SDL_PeepEvents");
    }

    int result = orig_SDL_PeepEvents(events, numevents, action, minType, maxType);

    if (action == SDL_GETEVENT && events && result > 0) {
        for (int i = 0; i < result; i++) {
            process_sdl_event(&events[i]);
        }
    }

    return result;
}

/* ─── OpenGL hook ─────────────────────────────────────────────── */

void glClear(GLbitfield mask) {
    /* Resolve original on first call */
    if (!orig_glClear) {
        orig_glClear = dlsym(RTLD_NEXT, "glClear");
        luma_init();
        LOG("glClear hook ACTIVE");
        LOG("Using SDL2 event hooking for keys");
    }

    g_frame_counter++;

    /* Periodic health log every 300 frames (~10 s at 30 fps) */
    if (g_frame_counter % 300 == 0) {
        LOG("Frame %d - UI: %s | SDL hooked: %s",
            g_frame_counter,
            luma_gui_is_open() ? "OPEN" : "CLOSED",
            g_sdl_hooked ? "YES" : "NO");
    }

    /* Render Luma overlay before the game clears the frame */
    if (luma_gui_is_open()) {
        luma_gui_render();
    }

    /* Always render HUD modules (even when menu is closed) */
    /* (luma_gui_render handles this distinction internally)  */
    if (!luma_gui_is_open()) {
        luma_gui_render(); /* renders HUD only */
    }

    orig_glClear(mask);
}

/* ─── Constructor / Destructor ────────────────────────────────── */

__attribute__((constructor))
static void luma_constructor(void) {
    LOG("Shared library loaded - constructor called");
    /* Initialization deferred to first glClear call */
}

__attribute__((destructor))
static void luma_destructor(void) {
    LOG("Shared library unloading");
    luma_gui_shutdown();
}
