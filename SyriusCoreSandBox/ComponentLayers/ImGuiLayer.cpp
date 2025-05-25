#include "ImGuiLayer.hpp"

ImGuiLayer::ImGuiLayer(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer* container):
IComponent(window, context, container){}

void ImGuiLayer::onImGui(ImGuiWindowData &windowData) {
    ImGui::ShowDemoWindow();

    if (m_Context->hasImPlotContext()) {
        ImPlot::ShowDemoWindow();
    }

}


