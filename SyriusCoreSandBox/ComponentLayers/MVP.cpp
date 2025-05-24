#include "MVP.hpp"

#include "../Components/ShaderComponent.hpp"
#include "../Core/ComponentContainer.hpp"

MVP::MVP(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    auto shader = container->getComponent<ShaderComponent>();
    shader->setShader("MVP", "MVP");
}

void MVP::onUpdate(Duration deltaTime) {

}

void MVP::onImGui(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "DepthTest");

    if (ImGui::Checkbox("Depth Test", &m_EnableDepthTest)) {
        m_Context->getDefaultFrameBuffer()->enableDepthTest(m_EnableDepthTest);
    }

    imGuiEnd(windowData);
}


