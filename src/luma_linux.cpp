// src/luma_linux.cpp
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <dlfcn.h>
#include <chrono>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"

static EGLBoolean (*LumaOrigEglSwapBuffers)(EGLDisplay, EGLSurface) = nullptr;
static bool g_lumaImguiInitialized = false;

void LumaOnTick(float dt);
void LumaOnRender();

static void LumaImguiInit() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();
    ImGui_ImplAndroid_Init(nullptr);
    ImGui_ImplOpenGL3_Init("#version 100");

    g_lumaImguiInitialized = true;
}

static EGLBoolean LumaHookEglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    static auto lastTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    float dt = std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;

    if (!g_lumaImguiInitialized)
        LumaImguiInit();

    LumaOnTick(dt);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

    LumaOnRender();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return LumaOrigEglSwapBuffers(dpy, surface);
}

static void LumaInstallHooksOnce() {
    static bool done = false;
    if (done) return;
    done = true;

    void *egl = dlopen("libEGL.so", RTLD_LAZY);
    if (!egl) return;
    void *sym = dlsym(egl, "eglSwapBuffers");
    if (!sym) return;
    LumaOrigEglSwapBuffers = (EGLBoolean (*)(EGLDisplay, EGLSurface))sym;

    // TODO: install your hook here using your preferred hooking method,
    // replacing eglSwapBuffers with LumaHookEglSwapBuffers.
}

__attribute__((constructor))
static void LumaInit() {
    LumaInstallHooksOnce();
}
