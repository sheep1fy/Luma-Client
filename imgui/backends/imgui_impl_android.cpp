// imgui/backends/imgui_impl_android.cpp
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include <android/input.h>
#include <android/log.h>
#include <dlfcn.h>

// Data for Android input
struct AndroidInputState {
    int screenWidth;
    int screenHeight;
    float mouseX, mouseY;
    bool mouseDown[5];
    bool keysDown[512];
};

static AndroidInputState g_input = {};
static bool g_initialized = false;

extern "C" {
    __attribute__((visibility("default"))) 
    bool ImGui_ImplAndroid_Init(ANativeWindow* window) {
        IM_UNUSED(window);
        
        g_initialized = true;
        g_input.screenWidth = 1280;   // TODO: Get from MCPelauncher
        g_input.screenHeight = 720;   // TODO: Get from MCPelauncher
        
        __android_log_print(ANDROID_LOG_INFO, "ImGuiAndroid", "Initialized");
        return true;
    }
    
    void ImGui_ImplAndroid_Shutdown() {
        g_initialized = false;
        ImGui_ImplOpenGL3_Shutdown();
        __android_log_print(ANDROID_LOG_INFO, "ImGuiAndroid", "Shutdown");
    }
    
    void ImGui_ImplAndroid_NewFrame() {
        if (!g_initialized)
            return;
        
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)g_input.screenWidth, (float)g_input.screenHeight);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        
        // Mouse
        io.MousePos = ImVec2(g_input.mouseX, g_input.mouseY);
        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
            io.MouseDown[i] = g_input.mouseDown[i];
        
        // Keyboard (map Android keys to ImGui keys)
        for (int i = 0; i < 512; i++)
            io.KeysDown[i] = g_input.keysDown[i];
            
        io.AddMousePosEvent(g_input.mouseX, g_input.mouseY);
    }
    
    bool ImGui_ImplAndroid_UpdateInput() {
        // TODO: Hook this to MCPelauncher input events
        // For now returns true (input ready)
        return true;
    }
}

// Exported functions for MCPelauncher to call
extern "C" {
    void ImGuiAndroid_OnTouch(float x, float y, int action) {
        if (!g_initialized) return;
        
        g_input.mouseX = x;
        g_input.mouseY = y;
        
        if (action == AMOTION_EVENT_ACTION_DOWN)
            g_input.mouseDown[0] = true;
        else if (action == AMOTION_EVENT_ACTION_UP)
            g_input.mouseDown[0] = false;
    }
    
    void ImGuiAndroid_OnKey(int keycode, bool down) {
        if (!g_initialized) return;
        
        // Map Android keycodes to ImGui keys
        int imguiKey = 0;
        switch (keycode) {
            case AKEYCODE_DPAD_UP:    imguiKey = ImGuiKey_UpArrow; break;
            case AKEYCODE_DPAD_DOWN:  imguiKey = ImGuiKey_DownArrow; break;
            case AKEYCODE_DPAD_LEFT:  imguiKey = ImGuiKey_LeftArrow; break;
            case AKEYCODE_DPAD_RIGHT: imguiKey = ImGuiKey_RightArrow; break;
            case AKEYCODE_ENTER:      imguiKey = ImGuiKey_Enter; break;
            case AKEYCODE_DEL:        imguiKey = ImGuiKey_Backspace; break;
            // Add more mappings...
        }
        
        if (imguiKey > 0 && imguiKey < 512)
            g_input.keysDown[imguiKey] = down;
    }
}
