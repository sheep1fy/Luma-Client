/*
 * luma_camera_api.cpp
 * Camera manipulation API stub.
 * Zoom module uses set_fov_scale to adjust the field-of-view.
 */

#include "luma_camera_api.h"
#include <cstdio>

static float g_fov_scale = 1.0f;
static bool  g_zoom_active = false;

void luma_camera_set_fov_scale(float scale) {
    g_fov_scale   = scale;
    g_zoom_active = (scale != 1.0f);
}

float luma_camera_get_fov_scale() {
    return g_fov_scale;
}

bool luma_camera_is_zoom_active() {
    return g_zoom_active;
}
