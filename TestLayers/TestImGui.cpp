#include "TestImGui.hpp"

TestImGui::TestImGui(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                     EasyIni::Configuration &config) : Layer(context, window, config) {
    m_Window->createImGuiContext();
}

TestImGui::~TestImGui() {
    m_Window->destroyImGuiContext();
}

void TestImGui::onUpdate() {
    m_Context->beginRenderPass();

    m_Window->onImGuiBegin();

    ImGui::Begin("Test ImGui");
    ImGui::Text("Hello World");
    ImGui::ColorPicker3("Background Color", m_Context->getDefaultFrameBuffer()->getColorAttachment(0)->getClearColor());
    ImGui::End();

    m_Window->onImGuiEnd();

    m_Context->endRenderPass();
}

void TestImGui::onEvent(const Event &event) {

}
