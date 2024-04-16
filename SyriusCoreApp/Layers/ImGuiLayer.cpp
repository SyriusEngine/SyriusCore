#include "ImGuiLayer.hpp"

ImGuiLayer::ImGuiLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config) {
    m_Window->createImGuiContext();
    addImGuiDrawFunction([this]{
        Layer::imGuiDebugPanel(m_Context);
    });

}

ImGuiLayer::~ImGuiLayer(){
    m_Window->destroyImGuiContext();
}

void ImGuiLayer::onUpdate() {
    m_Context->beginRenderPass();

    renderImGui();

    m_Context->endRenderPass();
}

void ImGuiLayer::onEvent(const Event &event) {

}

