// src/luma_game_api.cpp
#include "luma_game_api.h"

// TODO: replace these with real libminecraftpe / MCPelauncher calls

LumaVec3 LumaGetPlayerPosition() {
    return {0.f, 64.f, 0.f};
}

int LumaGetPingMs() {
    return 0;
}

bool LumaIsMovingForward() {
    return false;
}

bool LumaIsOnGround() {
    return true;
}

void LumaSetSprintKeyDown(bool) {
}

void LumaRegisterClick(bool) {
}
