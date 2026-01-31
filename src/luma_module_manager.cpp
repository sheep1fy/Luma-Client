// ============================================================================
// luma_module_manager.cpp - Modern UI with Glassmorphism & Custom Toggles
// ============================================================================
// This file implements a Flarial-style client interface with:
// - Sidebar navigation with tab selection
// - Custom animated toggle switches (no standard checkboxes)
// - Cyan accent color (#00CCFF) on deep charcoal background
// - Glassmorphism effect (semi-transparent panels)
// ============================================================================

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include <GLES3/gl3.h>
#include <vector>
#include <string>
#include <cmath>

// ============================================================================
// LUMA THEME - Cyan Accent on Dark Background
// ============================================================================
namespace LumaTheme {
    // Primary Colors
    static const ImVec4 Accent      = ImVec4(0.00f, 0.80f, 1.00f, 1.00f);  // Cyan #00CCFF
    static const ImVec4 AccentHover = ImVec4(0.00f, 0.90f, 1.00f, 1.00f);  // Brighter on hover
    static const ImVec4 AccentDim   = ImVec4(0.00f, 0.60f, 0.80f, 1.00f);  // Dimmed cyan
    
    // Backgrounds (Glassmorphism)
    static const ImVec4 WindowBg    = ImVec4(0.05f, 0.05f, 0.05f, 0.92f);  // Main window (92% opacity)
    static const ImVec4 Sidebar     = ImVec4(0.03f, 0.03f, 0.03f, 0.98f);  // Darker sidebar
    static const ImVec4 ChildBg     = ImVec4(0.08f, 0.08f, 0.08f, 0.75f);  // Module cards (75% opacity)
    
    // Text
    static const ImVec4 TextMain    = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);  // White
    static const ImVec4 TextDim     = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);  // Gray
    static const ImVec4 TextDisabled= ImVec4(0.40f, 0.40f, 0.40f, 1.00f);  // Dimmer gray
    
    // UI Elements
    static const ImVec4 Border      = ImVec4(0.15f, 0.15f, 0.15f, 0.50f);  // Subtle borders
    static const ImVec4 Separator   = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);  // Visible separators
}

// ============================================================================
// MODULE DATA STRUCTURE
// ============================================================================
struct LumaModule {
    std::string name;
    std::string description;
    bool enabled;
    std::string category; // "Combat", "Visual", "Movement", "Misc"
};

// ============================================================================
// MODULE REGISTRY
// ============================================================================
static std::vector<LumaModule> g_modules = {
    // Combat Modules
    {"AutoClicker",    "Automatically click at customizable CPS",              false, "Combat"},
    {"Velocity",       "Reduce knockback from attacks",                        false, "Combat"},
    {"Reach",          "Extend combat range by 0.5 blocks",                    false, "Combat"},
    
    // Visual Modules
    {"Keystrokes",     "Display WASD and mouse inputs on screen",              true,  "Visual"},
    {"CPS Counter",    "Show real-time left/right clicks per second",          true,  "Visual"},
    {"FPS Display",    "Show current framerate with smoothing",                true,  "Visual"},
    {"Coordinates",    "Display world position (X, Y, Z)",                     true,  "Visual"},
    {"Ping Display",   "Show network latency in milliseconds",                 true,  "Visual"},
    
    // Movement Modules
    {"ToggleSprint",   "Maintain sprint without holding key",                  false, "Movement"},
    {"AutoWalk",       "Automatically walk forward",                           false, "Movement"},
    {"FastLadder",     "Climb ladders 2x faster",                              false, "Movement"},
    
    // Misc Modules
    {"Zoom",           "Adjustable field-of-view for scouting",                false, "Misc"},
    {"AutoGG",         "Automatically send 'gg' after games",                  false, "Misc"},
    {"FullBright",     "Maximum brightness in dark areas",                     false, "Misc"},
};

// ============================================================================
// UI STATE
// ============================================================================
bool menu_open = false;           // Exported to C for luma_linux.c
static int active_tab = 0;        // 0=Modules, 1=Visuals, 2=Settings, 3=Info
static bool imgui_initialized = false;
static float animation_time = 0.0f;

// ============================================================================
// CUSTOM UI COMPONENT: Animated Toggle Switch
// ============================================================================
// Replaces standard ImGui checkboxes with a sleek iOS-style toggle
// Parameters:
//   label: Unique identifier for the widget
//   v: Pointer to boolean value to toggle
// Returns: true if the value was toggled
// ============================================================================
static bool RenderCustomToggle(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    
    // Toggle dimensions
    const float height = ImGui::GetFrameHeight() * 0.65f;
    const float width = height * 2.0f;
    const float radius = height * 0.5f;
    
    // Calculate position
    const ImVec2 pos = ImGui::GetCursorScreenPos();
    const ImRect total_bb(pos, ImVec2(pos.x + width, pos.y + height));
    
    // Handle input
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !*v;
        ImGui::MarkItemEdited(id);
    }
    
    // Animation (smooth transition)
    static float t = 0.0f;
    const float target = *v ? 1.0f : 0.0f;
    const float animation_speed = 8.0f;
    t = t + (target - t) * ImGui::GetIO().DeltaTime * animation_speed;
    
    // Render background track
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImU32 col_bg;
    if (*v) {
        col_bg = hovered ? ImGui::GetColorU32(LumaTheme::AccentHover) 
                        : ImGui::GetColorU32(LumaTheme::Accent);
    } else {
        col_bg = IM_COL32(40, 40, 40, 255);
    }
    
    draw_list->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), col_bg, radius);
    
    // Render sliding knob
    const float knob_x = pos.x + radius + (width - 2.0f * radius) * t;
    const float knob_y = pos.y + radius;
    
    // Shadow effect
    draw_list->AddCircleFilled(ImVec2(knob_x + 1, knob_y + 1), radius - 1.5f, 
                               IM_COL32(0, 0, 0, 50), 16);
    
    // White knob
    draw_list->AddCircleFilled(ImVec2(knob_x, knob_y), radius - 1.5f, 
                               IM_COL32(255, 255, 255, 255), 16);
    
    // Advance cursor
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    ImGui::ItemAdd(total_bb, id);
    
    return pressed;
}

// ============================================================================
// APPLY MODERN THEME
// ============================================================================
static void ApplyModernStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Rounding
    style.WindowRounding    = 12.0f;
    style.ChildRounding     = 10.0f;
    style.FrameRounding     = 6.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding      = 6.0f;
    style.TabRounding       = 6.0f;
    
    // Spacing
    style.WindowPadding     = ImVec2(0, 0);   // Manual padding for sidebar
    style.FramePadding      = ImVec2(10, 6);
    style.ItemSpacing       = ImVec2(10, 8);
    style.ItemInnerSpacing  = ImVec2(8, 6);
    style.IndentSpacing     = 20.0f;
    
    // Borders
    style.WindowBorderSize  = 0.0f;
    style.ChildBorderSize   = 1.0f;
    style.FrameBorderSize   = 0.0f;
    
    // Colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]         = LumaTheme::WindowBg;
    colors[ImGuiCol_ChildBg]          = LumaTheme::ChildBg;
    colors[ImGuiCol_Border]           = LumaTheme::Border;
    colors[ImGuiCol_Text]             = LumaTheme::TextMain;
    colors[ImGuiCol_TextDisabled]     = LumaTheme::TextDisabled;
    colors[ImGuiCol_FrameBg]          = ImVec4(0.12f, 0.12f, 0.12f, 0.80f);
    colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.15f, 0.15f, 0.15f, 0.90f);
    colors[ImGuiCol_FrameBgActive]    = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TitleBg]          = LumaTheme::Sidebar;
    colors[ImGuiCol_TitleBgActive]    = LumaTheme::Sidebar;
    colors[ImGuiCol_ScrollbarBg]      = ImVec4(0.05f, 0.05f, 0.05f, 0.50f);
    colors[ImGuiCol_ScrollbarGrab]    = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.90f);
    colors[ImGuiCol_ScrollbarGrabActive]  = LumaTheme::Accent;
    colors[ImGuiCol_Separator]        = LumaTheme::Separator;
    colors[ImGuiCol_Button]           = ImVec4(0.10f, 0.10f, 0.10f, 0.80f);
    colors[ImGuiCol_ButtonHovered]    = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonActive]     = LumaTheme::AccentDim;
    colors[ImGuiCol_Header]           = LumaTheme::AccentDim;
    colors[ImGuiCol_HeaderHovered]    = LumaTheme::Accent;
    colors[ImGuiCol_HeaderActive]     = LumaTheme::AccentHover;
}

// ============================================================================
// INITIALIZE IMGUI (OpenGL ES 3 Backend)
// ============================================================================
static void InitializeImGui() {
    if (imgui_initialized) return;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // Disable config files (we're in a sandboxed mod environment)
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    
    // Setup OpenGL ES 3 backend
    ImGui_ImplOpenGL3_Init("#version 300 es");
    
    // Apply theme
    ApplyModernStyle();
    
    imgui_initialized = true;
}

// ============================================================================
// RENDER MODULE CARD
// ============================================================================
static void RenderModuleCard(LumaModule& mod) {
    // Card container with glassmorphism
    ImGui::BeginChild(mod.name.c_str(), ImVec2(0, 75), true, 
                      ImGuiWindowFlags_NoScrollbar);
    
    // Module name (left side)
    ImGui::SetCursorPos(ImVec2(15, 12));
    ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::TextMain);
    ImGui::Text("%s", mod.name.c_str());
    ImGui::PopStyleColor();
    
    // Description (left side, below name)
    ImGui::SetCursorPos(ImVec2(15, 35));
    ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::TextDim);
    ImGui::TextWrapped("%s", mod.description.c_str());
    ImGui::PopStyleColor();
    
    // Category tag (bottom left)
    ImGui::SetCursorPos(ImVec2(15, 55));
    ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::AccentDim);
    ImGui::TextUnformatted(mod.category.c_str());
    ImGui::PopStyleColor();
    
    // Toggle switch (right side, vertically centered)
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 65, 28));
    RenderCustomToggle(("##toggle_" + mod.name).c_str(), &mod.enabled);
    
    ImGui::EndChild();
}

// ============================================================================
// C INTERFACE FUNCTIONS (Called from luma_linux.c)
// ============================================================================
extern "C" {

// Initialize the module manager
void init_luma_manager() {
    InitializeImGui();
}

// Toggle menu visibility (triggered by 'K' key)
void toggle_luma_menu() {
    menu_open = !menu_open;
}

// Main UI render tick (called every frame from ScreenContext::render hook)
void run_luma_tick() {
    if (!menu_open) return;
    
    // Start new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    
    animation_time += ImGui::GetIO().DeltaTime;
    
    // ========================================================================
    // MAIN WINDOW
    // ========================================================================
    ImGui::SetNextWindowSize(ImVec2(750, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("##LumaClient", &menu_open, 
                 ImGuiWindowFlags_NoTitleBar | 
                 ImGuiWindowFlags_NoResize | 
                 ImGuiWindowFlags_NoCollapse);
    
    // ========================================================================
    // LEFT SIDEBAR (Navigation)
    // ========================================================================
    ImGui::BeginChild("##Sidebar", ImVec2(180, 0), true, 
                      ImGuiWindowFlags_NoScrollbar);
    
    // Logo section
    ImGui::SetCursorPosY(25);
    ImGui::SetCursorPosX(20);
    ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::Accent);
    ImGui::Text("LUMA");
    ImGui::PopStyleColor();
    
    ImGui::SetCursorPosX(20);
    ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::TextDim);
    ImGui::TextUnformatted("CLIENT");
    ImGui::PopStyleColor();
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();
    
    // Navigation tabs
    const char* tabs[] = {"Modules", "Visuals", "Settings", "Info"};
    const char* icons[] = {"⚙", "👁", "🔧", "ℹ"}; // Unicode icons
    
    for (int i = 0; i < 4; i++) {
        bool selected = (active_tab == i);
        
        if (selected) {
            ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::Accent);
            ImGui::PushStyleColor(ImGuiCol_Header, LumaTheme::AccentDim);
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::TextMain);
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
        }
        
        // Tab button with icon
        char label[64];
        snprintf(label, sizeof(label), " %s  %s", icons[i], tabs[i]);
        
        if (ImGui::Selectable(label, selected, 0, ImVec2(0, 40))) {
            active_tab = i;
        }
        
        ImGui::PopStyleColor(2);
        ImGui::Spacing();
    }
    
    // Footer (version info)
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);
    ImGui::Separator();
    ImGui::SetCursorPosX(20);
    ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::TextDisabled);
    ImGui::TextUnformatted("v1.0.1");
    ImGui::PopStyleColor();
    
    ImGui::EndChild();
    
    // ========================================================================
    // RIGHT CONTENT AREA
    // ========================================================================
    ImGui::SameLine();
    
    ImGui::BeginGroup();
    ImGui::SetCursorPos(ImVec2(200, 20));
    
    // Header
    ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::TextDim);
    ImGui::Text("LUMA CLIENT | %s", tabs[active_tab]);
    ImGui::PopStyleColor();
    
    ImGui::SetCursorPosX(200);
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 10));
    
    // Scrollable content
    ImGui::SetCursorPosX(200);
    ImGui::BeginChild("##ContentScroll", ImVec2(530, 420), false);
    
    // ========================================================================
    // TAB CONTENT
    // ========================================================================
    if (active_tab == 0) {
        // Modules Tab - Show all modules as cards
        for (auto& mod : g_modules) {
            RenderModuleCard(mod);
            ImGui::Spacing();
        }
    }
    else if (active_tab == 1) {
        // Visuals Tab
        ImGui::TextColored(LumaTheme::Accent, "HUD Customization");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextWrapped("Customize the position and appearance of HUD elements.");
        ImGui::Spacing();
        ImGui::Button("Open HUD Editor", ImVec2(200, 35));
        ImGui::Spacing();
        ImGui::Spacing();
        
        ImGui::TextColored(LumaTheme::Accent, "Theme Settings");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextWrapped("Color scheme and transparency options coming soon.");
    }
    else if (active_tab == 2) {
        // Settings Tab
        ImGui::TextColored(LumaTheme::Accent, "Client Settings");
        ImGui::Separator();
        ImGui::Spacing();
        
        static bool auto_update = true;
        ImGui::Text("Auto-Update");
        ImGui::SameLine(300);
        RenderCustomToggle("##autoupdate", &auto_update);
        ImGui::Spacing();
        
        static bool telemetry = false;
        ImGui::Text("Anonymous Telemetry");
        ImGui::SameLine(300);
        RenderCustomToggle("##telemetry", &telemetry);
        ImGui::Spacing();
        
        ImGui::Spacing();
        ImGui::Button("Reset All Settings", ImVec2(200, 35));
    }
    else if (active_tab == 3) {
        // Info Tab
        ImGui::TextColored(LumaTheme::Accent, "About Luma Client");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextWrapped("Luma Client is a high-performance utility mod for Minecraft Bedrock Edition on Linux.");
        ImGui::Spacing();
        ImGui::TextWrapped("Version: 1.0.1");
        ImGui::TextWrapped("Build: " __DATE__ " " __TIME__);
        ImGui::TextWrapped("Platform: mcpelauncher (Linux x86_64)");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextColored(LumaTheme::Accent, "Credits");
        ImGui::BulletText("Developed by sheep1fy");
        ImGui::BulletText("UI powered by Dear ImGui");
        ImGui::BulletText("Hooking via Dobby library");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::TextColored(LumaTheme::TextDim, "GitHub: github.com/sheep1fy/Luma-Client");
    }
    
    ImGui::EndChild(); // ContentScroll
    ImGui::EndGroup();
    
    ImGui::End(); // Main window
    
    // ========================================================================
    // RENDER IMGUI
    // ========================================================================
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // extern "C"

int DobbyHook(void* target, void* replace, void** result) {
    *result = target;  // No-op for now
    return 0;
}
