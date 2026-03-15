/*
 * luma_game_api.cpp
 * Game state access stub.
 * Real coordinates/ping require hooking Minecraft internals via symbol offsets.
 * These stubs return placeholder values; replace with real hooks for your target version.
 */

#include "luma_game_api.h"
#include <cmath>
#include <ctime>

/* ── Coordinates ─────────────────────────────────────────────── */
static double g_player_x = 0.0;
static double g_player_y = 64.0;
static double g_player_z = 0.0;

double luma_game_get_x() { return g_player_x; }
double luma_game_get_y() { return g_player_y; }
double luma_game_get_z() { return g_player_z; }

void luma_game_set_coords(double x, double y, double z) {
    g_player_x = x;
    g_player_y = y;
    g_player_z = z;
}

/* ── Ping ─────────────────────────────────────────────────────── */
static int g_ping_ms = 0;

int  luma_game_get_ping() { return g_ping_ms; }
void luma_game_set_ping(int ms) { g_ping_ms = ms; }

/* ── Sprint ───────────────────────────────────────────────────── */
static bool g_auto_sprint = false;
bool luma_game_is_auto_sprint() { return g_auto_sprint; }
void luma_game_set_auto_sprint(bool v) { g_auto_sprint = v; }
