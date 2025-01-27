#include "DebugInfo.hpp"

DebugInfo::DebugInfo(UP<SyriusWindow>& window, ResourceView<Context> &context) :
AppComponent(window, context) {

}

void DebugInfo::onImGuiRender(ImGuiSizeData &sizeData) {
    AppComponent::imGuiBeginPanel(sizeData, "Debug Info");

    ImGui::ColorPicker3("Background Color", m_Context->getClearColor());

    AppComponent::imGuiEndPanel(sizeData);
}
