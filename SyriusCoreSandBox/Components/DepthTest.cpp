#include "DepthTest.hpp"

DepthTest::DepthTest(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){

}

void DepthTest::onImGui(ImGuiWindowData &windowData) {
    imGuiBegin(windowData, "Depth Test");

    if (ImGui::Checkbox("Depth Testing", &m_EnableDepthTest)) {
        m_Context->getDefaultFrameBuffer()->enableDepthTest(m_EnableDepthTest);
    }

    imGuiEnd(windowData);
}
