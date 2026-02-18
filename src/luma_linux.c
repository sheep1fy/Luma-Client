// ============================================================================
// luma_linux.c - SDL2 Event Hook for Wayland/X11 Compatibility
// ============================================================================

#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <string.h>
#include "luma_gui.h"

#ifdef ANDROID
    #include <android/log.h>
    #define LOG_TAG "LumaClient"
    #define LUMA_LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#else
    #define LUMA_LOG(...) fprintf(stderr, "[LumaClient] " __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr)
#endif

/* =========================
   OpenGL / GLES Compatibility
   ========================= */

#ifdef ANDROID
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #ifndef glColor4f
        #define glColor4f(r, g, b, a) ((void)0)
    #endif
#else
    #include <GL/gl.h>
#endif

/* =========================
   SDL2 Minimal Definitions
   ========================= */

// Minimal SDL2 event structures (we don't need full SDL headers)
typedef enum {
    SDL_KEYDOWN = 0x300,
    SDL_KEYUP = 0x301
} SDL_EventType;

typedef struct SDL_Keysym {
    uint32_t scancode;
    int32_t sym;
    uint16_t mod;
    uint32_t unused;
} SDL_Keysym;

typedef struct SDL_KeyboardEvent {
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint8_t state;
    uint8_t repeat;
    uint8_t padding2;
    uint8_t padding3;
    SDL_Keysym keysym;
} SDL_KeyboardEvent;

typedef union SDL_Event {
    uint32_t type;
    SDL_KeyboardEvent key;
    uint8_t padding[56];
} SDL_Event;

// SDL key codes
#define SDLK_k 107
#define SDLK_K 107  // Same as lowercase

/* =========================
   C ↔ C++ Bridges
   ========================= */

#ifdef __cplusplus
extern "C" {
#endif

void luma_gui_render(void);
void luma_core_init(void);

#ifdef __cplusplus
}
#endif

/* =========================
   Globals
   ========================= */

static bool ui_open = false;
static void (*orig_glClear)(GLbitfield mask) = NULL;
static int frame_count = 0;
static bool first_frame_logged = false;
static bool hook_active = false;

typedef int (*SDL_PollEvent_t)(SDL_Event *event);
static SDL_PollEvent_t orig_SDL_PollEvent = NULL;

typedef int (*SDL_PeepEvents_t)(SDL_Event *events, int numevents, int action, uint32_t minType, uint32_t maxType);
static SDL_PeepEvents_t orig_SDL_PeepEvents = NULL;

/* =========================
   SDL2 Event Hooks (Wayland Compatible)
   ========================= */

int SDL_PollEvent(SDL_Event *event) {
    // Get original function
    if (!orig_SDL_PollEvent) {
        orig_SDL_PollEvent = (SDL_PollEvent_t)dlsym(RTLD_NEXT, "SDL_PollEvent");
        if (!orig_SDL_PollEvent) {
            LUMA_LOG("ERROR: Could not find SDL_PollEvent");
            return 0;
        }
        LUMA_LOG("Hooked SDL_PollEvent successfully");
    }
    
    // Call original
    int result = orig_SDL_PollEvent(event);
    
    // Check if we got an event
    if (result && event) {
        // Check for K key press
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_k) {
            // Check if not repeat
            if (event->key.repeat == 0) {
                ui_open = !ui_open;
                LUMA_LOG("*** K KEY PRESSED (SDL2) - UI toggled to: %s ***", ui_open ? "OPEN" : "CLOSED");
            }
        }
    }
    
    return result;
}

int SDL_PeepEvents(SDL_Event *events, int numevents, int action, uint32_t minType, uint32_t maxType) {
    // Get original function
    if (!orig_SDL_PeepEvents) {
        orig_SDL_PeepEvents = (SDL_PeepEvents_t)dlsym(RTLD_NEXT, "SDL_PeepEvents");
        if (!orig_SDL_PeepEvents) {
            return 0;
        }
        LUMA_LOG("Hooked SDL_PeepEvents successfully");
    }
    
    // Call original
    int result = orig_SDL_PeepEvents(events, numevents, action, minType, maxType);
    
    // Check events if we got any
    if (result > 0 && events) {
        for (int i = 0; i < result; i++) {
            if (events[i].type == SDL_KEYDOWN && events[i].key.keysym.sym == SDLK_k) {
                if (events[i].key.repeat == 0) {
                    ui_open = !ui_open;
                    LUMA_LOG("*** K KEY PRESSED (SDL_PeepEvents) - UI toggled to: %s ***", ui_open ? "OPEN" : "CLOSED");
                }
            }
        }
    }
    
    return result;
}

/* =========================
   Hooked glClear (for rendering)
   ========================= */

void glClear(GLbitfield mask) {
    frame_count++;
    
    // Log first frame
    if (!first_frame_logged) {
        LUMA_LOG("======================================");
        LUMA_LOG("Luma Client glClear hook ACTIVE!");
        LUMA_LOG("Using SDL2 event hooking for keys");
        LUMA_LOG("Press 'K' to toggle menu");
        LUMA_LOG("======================================");
        first_frame_logged = true;
        hook_active = true;
    }
    
    // Log status every 5 seconds
    if (frame_count % 300 == 0) {
        LUMA_LOG("Frame %d - Hook active, UI: %s, SDL hooked: %s", 
                 frame_count, 
                 ui_open ? "OPEN" : "CLOSED",
                 orig_SDL_PollEvent ? "YES" : "NO");
    }
    
    // Get original glClear
    if (!orig_glClear) {
        orig_glClear = (void (*)(GLbitfield))dlsym(RTLD_NEXT, "glClear");
        if (!orig_glClear) {
            LUMA_LOG("ERROR: Could not find original glClear");
            return;
        }
    }

    // Render GUI if open
    if (ui_open) {
        glColor4f(0.f, 0.f, 0.f, 0.4f);
        luma_gui_render();
    }

    // Call original
    orig_glClear(mask);
}

/* =========================
   Library Init
   ========================= */

__attribute__((constructor))
static void luma_init() {
    LUMA_LOG("======================================");
    LUMA_LOG("Luma Client v2.0.0 SDL2 - Initializing");
    LUMA_LOG("Wayland/X11 compatible input hooking");
    LUMA_LOG("======================================");
    luma_core_init();
    LUMA_LOG("Core initialization complete");
}

/* =========================
   Cleanup
   ========================= */

__attribute__((destructor))
static void luma_cleanup() {
    LUMA_LOG("Luma Client shutting down...");
    LUMA_LOG("Total frames rendered: %d", frame_count);
}
