// src/luma_camera_api.cpp
#include "luma_camera_api.h"
#include "../apis/bedrock_1_21_30.hpp"

static bool g_cameraReady = false;

static void LumaInitCameraAPI() {
    if (g_cameraReady) return;
    g_cameraReady = bedrock_1_21_30::init();
}

float LumaGetCurrentFOV() {
    LumaInitCameraAPI();
    if (!g_cameraReady || !bedrock_1_21_30::is_ready())
        return 70.f;
    return bedrock_1_21_30::get_current_fov();
}

void LumaSetCurrentFOV(float fov) {
    LumaInitCameraAPI();
    if (g_cameraReady && bedrock_1_21_30::is_ready())
        bedrock_1_21_30::set_current_fov(fov);
}
