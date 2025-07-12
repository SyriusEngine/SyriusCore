#include "ImGuiLayer.hpp"

ImGuiLayer::ImGuiLayer(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer* container):
IComponent(window, context, container){}

void ImGuiLayer::onImGui(ImGuiWindowData &windowData) {
    ImGui::ShowDemoWindow();

    if (m_Context->hasImPlotContext()) {
        ImPlot::ShowDemoWindow();
    }

    static std::string s_Text = "Example";
    ImGui::Begin("std::string text input");
    ImGui::InputText("Some Text Input", &s_Text);
    ImGui::End();
}


