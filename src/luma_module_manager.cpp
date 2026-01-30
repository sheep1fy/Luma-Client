#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include <string>

// --- Luma Palette (Flarial-Inspired) ---
namespace LumaTheme {
    static ImVec4 Accent      = ImVec4(0.00f, 0.75f, 1.00f, 1.00f); // Bright Cyan
    static ImVec4 Background  = ImVec4(0.05f, 0.05f, 0.05f, 0.85f); // Glassy Dark
    static ImVec4 Sidebar     = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
    static ImVec4 TextMain    = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    static ImVec4 TextDim     = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
}

struct Module {
    std::string name;
    std::string desc;
    bool enabled;
};

std::vector<Module> modules = {
    {"ToggleSprint", "Maintain sprinting without holding key", false},
    {"Keystrokes", "Display WASD movement on screen", true},
    {"CPS Counter", "Real-time clicks per second display", true},
    {"ArmorHUD", "View armor durability and items", false},
    {"MiniMap", "Legit top-down radar view", false}
};

bool menu_open = false;
int active_tab = 0;

// --- Custom Modern UI Components ---
void RenderToggle(const char* label, bool* v) {
    ImGui::Text("%s", label);
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 40);
    
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight() * 0.6f;
    float width = height * 2.0f;
    float radius = height * 0.5f;

    if (ImGui::InvisibleButton(label, ImVec2(width, height)))
        *v = !*v;

    ImU32 col_bg = *v ? ImGui::GetColorU32(LumaTheme::Accent) : IM_COL32(50, 50, 50, 255);
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, radius);
    draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

void ApplySleekStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.WindowPadding = ImVec2(0, 0); // Handle padding manually for sidebar
    style.ItemSpacing = ImVec2(10, 10);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = LumaTheme::Background;
    colors[ImGuiCol_Border]   = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
}

extern "C" {
    void init_luma_manager() { /* Loaded via constructor */ }
    void toggle_luma_menu() { menu_open = !menu_open; }

    void run_luma_tick() {
        if (!menu_open) return;

        ApplySleekStyle();
        ImGui::SetNextWindowSize(ImVec2(650, 420), ImGuiCond_Once);

        if (ImGui::Begin("LumaClient", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
            
            // --- Left Sidebar ---
            ImGui::BeginChild("Sidebar", ImVec2(160, 0), true, ImGuiWindowFlags_NoScrollbar);
            ImGui::SetCursorPosY(20);
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Assumes custom font loaded
            ImGui::Indent(20);
            ImGui::TextColored(LumaTheme::Accent, "LUMA");
            ImGui::Unindent(20);
            ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

            const char* tabs[] = {"Modules", "Visuals", "Configs", "Info"};
            for (int i = 0; i < 4; i++) {
                bool selected = (active_tab == i);
                if (selected) ImGui::PushStyleColor(ImGuiCol_Text, LumaTheme::Accent);
                
                if (ImGui::Selectable(tabs[i], selected, 0, ImVec2(0, 35))) active_tab = i;
                
                if (selected) ImGui::PopStyleColor();
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // --- Content Area ---
            ImGui::BeginGroup();
            ImGui::SetCursorPos(ImVec2(180, 20));
            ImGui::TextDisabled("LUMA CLIENT | %s", tabs[active_tab]);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 10));

            ImGui::BeginChild("ScrollableContent", ImVec2(0, 0), false);
            if (active_tab == 0) { // Modules
                for (auto& mod : modules) {
                    ImGui::BeginChild(mod.name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 10, 60), true);
                    ImGui::SetCursorPos(ImVec2(10, 10));
                    ImGui::Text("%s", mod.name.c_str());
                    ImGui::SetCursorPos(ImVec2(10, 30));
                    ImGui::TextDisabled("%s", mod.desc.c_str());
                    
                    ImGui::SetCursorPos(ImVec2(0, 0)); // Reset for Toggle alignment
                    RenderToggle(mod.name.c_str(), &mod.enabled);
                    ImGui::EndChild();
                }
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
        ImGui::End();
    }
}
