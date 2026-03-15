/*
 * luma_gui.cpp
 * Luma Client - ImGui rendering interface + C bridge
 */

#include <luma_gui.h>

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

#include "luma_module_manager.h"
#include "luma_hud_editor.h"

#include <cstdio>
#include <string>

/* ─── Globals ─────────────────────────────────────────────────── */
bool LumaHudEditMode = false;
bool LumaMenuOpen    = false;

static bool g_imgui_initialized = false;

/* ─── Private helpers ─────────────────────────────────────────── */

static void init_imgui() {
    if (g_imgui_initialized) return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr; /* disable imgui.ini */

    /* Style */
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding    = 8.0f;
    style.FrameRounding     = 4.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding      = 4.0f;
    style.TabRounding       = 4.0f;
    style.WindowBorderSize  = 1.0f;
    style.FrameBorderSize   = 0.0f;

    /* Luma purple/dark colour palette */
    ImVec4 *colors = style.Colors;
    colors[ImGuiCol_WindowBg]         = ImVec4(0.08f, 0.08f, 0.10f, 0.95f);
    colors[ImGuiCol_TitleBg]          = ImVec4(0.13f, 0.07f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBgActive]    = ImVec4(0.20f, 0.10f, 0.35f, 1.00f);
    colors[ImGuiCol_Header]           = ImVec4(0.25f, 0.13f, 0.40f, 0.80f);
    colors[ImGuiCol_HeaderHovered]    = ImVec4(0.35f, 0.18f, 0.55f, 0.90f);
    colors[ImGuiCol_HeaderActive]     = ImVec4(0.45f, 0.23f, 0.65f, 1.00f);
    colors[ImGuiCol_Button]           = ImVec4(0.25f, 0.13f, 0.40f, 0.80f);
    colors[ImGuiCol_ButtonHovered]    = ImVec4(0.35f, 0.18f, 0.55f, 0.90f);
    colors[ImGuiCol_ButtonActive]     = ImVec4(0.45f, 0.23f, 0.65f, 1.00f);
    colors[ImGuiCol_FrameBg]          = ImVec4(0.15f, 0.08f, 0.22f, 0.80f);
    colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.22f, 0.12f, 0.32f, 0.90f);
    colors[ImGuiCol_FrameBgActive]    = ImVec4(0.30f, 0.15f, 0.45f, 1.00f);
    colors[ImGuiCol_CheckMark]        = ImVec4(0.75f, 0.45f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab]       = ImVec4(0.55f, 0.28f, 0.80f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.70f, 0.38f, 1.00f, 1.00f);
    colors[ImGuiCol_Tab]              = ImVec4(0.13f, 0.07f, 0.20f, 0.90f);
    colors[ImGuiCol_TabHovered]       = ImVec4(0.30f, 0.15f, 0.45f, 1.00f);
    colors[ImGuiCol_TabActive]        = ImVec4(0.22f, 0.11f, 0.35f, 1.00f);
    colors[ImGuiCol_Separator]        = ImVec4(0.40f, 0.20f, 0.60f, 0.50f);

    /* OpenGL ES 2 backend (no input backend - SDL2 hook handles input) */
    ImGui_ImplOpenGL3_Init("#version 100");

    /* Set up a reasonable display size (updated each frame ideally) */
    io.DisplaySize = ImVec2(1920, 1080);

    g_imgui_initialized = true;
    fprintf(stderr, "[LumaClient] ImGui initialized (OpenGL ES2 backend)\n");
}

static void render_main_menu() {
    ImGui::SetNextWindowSize(ImVec2(480, 520), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(60, 60), ImGuiCond_Once);

    ImGui::Begin("✦ Luma Client  v2.0.0", &LumaMenuOpen,
                 ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored(ImVec4(0.75f, 0.45f, 1.00f, 1.00f),
                       "Minecraft Bedrock Client Mod");
    ImGui::TextDisabled("Press K to close this menu");
    ImGui::Separator();
    ImGui::Spacing();

    /* ── Tabs ── */
    if (ImGui::BeginTabBar("MainTabs")) {

        /* Modules tab */
        if (ImGui::BeginTabItem("Modules")) {
            ImGui::Spacing();
            auto &modules = LumaGetModules();
            for (auto *mod : modules) {
                ImGui::PushID(mod->name.c_str());
                bool enabled = mod->enabled;
                if (ImGui::Checkbox(mod->name.c_str(), &enabled)) {
                    mod->enabled = enabled;
                }
                if (mod->renderSettings && ImGui::CollapsingHeader(
                        ("  Settings##" + mod->name).c_str())) {
                    ImGui::Indent();
                    mod->renderSettings();
                    ImGui::Unindent();
                }
                ImGui::PopID();
                ImGui::Spacing();
            }
            ImGui::EndTabItem();
        }

        /* HUD Editor tab */
        if (ImGui::BeginTabItem("HUD Editor")) {
            ImGui::Spacing();
            ImGui::Checkbox("Enable HUD edit mode (drag modules)", &LumaHudEditMode);
            ImGui::Spacing();
            ImGui::TextDisabled("Drag module windows to reposition them.");
            ImGui::TextDisabled("Changes save automatically.");
            ImGui::EndTabItem();
        }

        /* Info tab */
        if (ImGui::BeginTabItem("Info")) {
            ImGui::Spacing();
            ImGui::Text("Luma Client  v2.0.0");
            ImGui::Text("Platform: Linux x86_64 (mcpelauncher)");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.5f,1.0f,0.5f,1.0f), "Hooks active:");
            ImGui::BulletText("glClear (render hook)");
            ImGui::BulletText("SDL_PollEvent (input hook)");
            ImGui::BulletText("SDL_PeepEvents (input hook)");
            ImGui::Spacing();
            ImGui::TextDisabled("github.com/sheep1fy/Luma-Client");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

static void render_hud() {
    auto &modules = LumaGetModules();
    for (auto *mod : modules) {
        if (!mod->enabled) continue;
        if (mod->renderHud) {
            mod->renderHud();
        }
    }
}

/* ─── C++ public API ──────────────────────────────────────────── */

void LumaGUIInit() {
    init_imgui();
}

void LumaGUIShutdown() {
    if (!g_imgui_initialized) return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    g_imgui_initialized = false;
}

void LumaGUIRender() {
    if (!g_imgui_initialized) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    /* Always render HUD modules */
    render_hud();

    /* Render main menu only when open */
    if (LumaMenuOpen) {
        render_main_menu();
    }

    /* HUD editor overlay */
    if (LumaHudEditMode) {
        LumaHudEditorRender();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/* ─── C API bridge ────────────────────────────────────────────── */

extern "C" {

void luma_gui_init(void) {
    LumaGUIInit();
}

void luma_gui_shutdown(void) {
    LumaGUIShutdown();
}

void luma_gui_render(void) {
    LumaGUIRender();
}

int luma_gui_is_open(void) {
    return LumaMenuOpen ? 1 : 0;
}

void luma_gui_toggle(void) {
    LumaMenuOpen = !LumaMenuOpen;
}

} /* extern "C" */
