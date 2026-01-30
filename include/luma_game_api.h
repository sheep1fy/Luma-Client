// include/luma_game_api.h
#pragma once

struct LumaVec3 {
    float x, y, z;
};

LumaVec3 LumaGetPlayerPosition();
int      LumaGetPingMs();
bool     LumaIsMovingForward();
bool     LumaIsOnGround();
void     LumaSetSprintKeyDown(bool down);
void     LumaRegisterClick(bool left);
