// imgui/backends/imgui_impl_android.h
#pragma once
#include "../imgui.h"
#include <android/native_window.h>

bool     ImGui_ImplAndroid_Init(ANativeWindow* window);
void     ImGui_ImplAndroid_Shutdown();
void     ImGui_ImplAndroid_NewFrame();
bool     ImGui_ImplAndroid_UpdateInput();
