// src/luma_camera_api.cpp 
#include "luma_camera_api.h"

// TODO: wire to real camera FOV
static float g_lumaFov = 70.f;

float LumaGetCurrentFOV() {
    return g_lumaFov;
}

void LumaSetCurrentFOV(float fov) {
    g_lumaFov = fov;
}
