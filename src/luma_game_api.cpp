// src/luma_game_api.cpp
#include "luma_game_api.h"
#include "../apis/bedrock_1_21_30.hpp"

static bool g_apiReady = false;

static void LumaInitBedrockAPI() {
    if (g_apiReady) return;
    g_apiReady = bedrock_1_21_30::init();
}

LumaVec3 LumaGetPlayerPosition() {
    LumaInitBedrockAPI();
    if (!g_apiReady || !bedrock_1_21_30::is_ready())
        return {0.f, 64.f, 0.f};
    return bedrock_1_21_30::get_player_position();
}

bool LumaIsOnGround() {
    LumaInitBedrockAPI();
    if (!g_apiReady || !bedrock_1_21_30::is_ready())
        return false;
    return bedrock_1_21_30::is_on_ground();
}

bool LumaIsMovingForward() {
    LumaInitBedrockAPI();
    if (!g_apiReady || !bedrock_1_21_30::is_ready())
        return false;
    return bedrock_1_21_30::is_moving_forward();
}

int LumaGetPingMs() {
    LumaInitBedrockAPI();
    if (!g_apiReady || !bedrock_1_21_30::is_ready())
        return 0;
    return bedrock_1_21_30::get_ping_ms();
}

void LumaSetSprintKeyDown(bool down) {
    LumaInitBedrockAPI();
    if (g_apiReady && bedrock_1_21_30::is_ready())
        bedrock_1_21_30::set_sprint_key_down(down);
}

void LumaRegisterAttackClick(bool left) {
    // CPS tracking lives in the CPS module, just forward there
    // (implement LumaCpsOnClick in luma_cps_module.cpp)
}
