// apis/bedrock_1_21_30.hpp
#pragma once
#include "../include/luma_game_api.h"
#include "../include/luma_camera_api.h"
#include <cstdint>

namespace bedrock_1_21_30 {

bool init();
bool is_ready();

// Game state
LumaVec3 get_player_position();
bool     is_on_ground();
bool     is_moving_forward();
int      get_ping_ms();
void     set_sprint_key_down(bool down);

// Camera
float    get_current_fov();
void     set_current_fov(float fov);

} // namespace bedrock_1_21_30
