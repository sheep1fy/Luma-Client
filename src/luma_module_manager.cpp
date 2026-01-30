#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include <string>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "LUMA", __VA_ARGS__)

// --- Module Structure ---
struct Module {
    std::string name;
    bool enabled;
    std::string description;
};

std::vector<Module> modules;
bool menu_open = false;
int current_tab = 0;

// --- Modern Theme Styling ---
void ApplyLumaStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;
    style.FrameRounding = 6.0f;
    style.ChildRounding = 8.0f;
    style.WindowPadding = ImVec2(20, 20);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 0.98f); // Sleek Dark
    colors[ImGuiCol_Border]   = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
    colors[ImGuiCol_Header]   = ImVec4(0.24f, 0.47f, 0.81f, 1.00f); // Luma Blue
    colors[ImGuiCol_Button]   = ImVec4(0.12f, 0.12f, 0.16f, 1.00f);
}

// --- C-Bridge Implementations ---
extern "C" {
    void init_luma_manager() {
        modules.push_back({"ToggleSprint", false, "Always sprint without holding keys."});
        modules.push_back({"Keystrokes", true, "Show WASD on your screen."});
        modules.push_back({"CPS Counter", true, "Displays your Clicks Per Second."});
        LOGI("Luma Manager Initialized with %zu modules", modules.size());
    }

    void toggle_luma_menu() {
        menu_open = !menu_open;
    }

    void run_luma_tick() {
        if (!menu_open) return;

        ApplyLumaStyle();
        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
        
        if (ImGui::Begin("LUMA CLIENT", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
            // --- Sidebar Navigation ---
            ImGui::BeginChild("Sidebar", ImVec2(140, 0), true);
            if (ImGui::Selectable("Modules", current_tab == 0)) current_tab = 0;
            if (ImGui::Selectable("Visuals", current_tab == 1)) current_tab = 1;
            if (ImGui::Selectable("Settings", current_tab == 2)) current_tab = 2;
            ImGui::EndChild();

            ImGui::SameLine();

            // --- Main Content Area ---
            ImGui::BeginGroup();
            ImGui::TextColored(ImVec4(0.3f, 0.6f, 1.0f, 1.0f), "LUMA LEGIT PVP");
            ImGui::Separator();

            if (current_tab == 0) {
                for (auto& mod : modules) {
                    ImGui::Checkbox(mod.name.c_str(), &mod.enabled);
                    ImGui::TextDisabled("  %s", mod.description.c_str());
                    ImGui::Spacing();
                }
            }
            ImGui::EndGroup();
        }
        ImGui::End();
    }
}
