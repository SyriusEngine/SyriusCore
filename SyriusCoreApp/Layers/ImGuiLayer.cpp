#include "ImGuiLayer.hpp"

ImGuiLayer::ImGuiLayer(ResourceView<Context> &context, const UP<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config) {

}

ImGuiLayer::~ImGuiLayer(){
}

void ImGuiLayer::onUpdate() {
    Layer::onUpdate();

    m_Context->clear();
    m_Context->beginRenderPass();

    renderImGui();

    m_Context->endRenderPass();
}

void ImGuiLayer::onEvent(const Event &event) {

}

