#include "BasicLayer.hpp"

BasicLayer::BasicLayer(ResourceView<Context> &context, const Resource<Window> &window, EasyIni::Configuration &config):
Layer(context, window, config) {

}

BasicLayer::~BasicLayer() {

}

void BasicLayer::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Sampler->bind(0);
    m_Texture->bind(0);
    m_Context->draw(m_VertexArray);

    m_Context->endRenderPass();

    m_Window->onImGuiBegin();

    ImGui::Begin("SyriusCoreDev");
    ImGui::Text("Graphics API: %s", getAPIName(m_Context->getType()).c_str());
    ImGui::ColorPicker3("Background Color", m_Context->getDefaultFrameBuffer()->getColorAttachment(0)->getClearColor());

    ImGui::End();

    m_Window->onImGuiEnd();
}

void BasicLayer::onEvent(const Event &event) {

}

void BasicLayer::onAttach() {
    m_Window->createImGuiContext();

    auto rectangle = createRectangle();
    m_ShaderProgram = m_ShaderLibrary.loadShader("Basic");
    m_VertexArray = loadMesh(rectangle, m_ShaderProgram);

    SamplerDesc samplerDesc;
    m_Sampler = m_Context->createSampler(samplerDesc);
    ImageFileDesc imgDesc;
    imgDesc.fileName = "./Resources/Textures/awesomeface.png";
    auto image = createImage(imgDesc);
    Texture2DImageDesc t2dDesc(image);
    m_Texture = m_Context->createTexture2D(t2dDesc);

}

void BasicLayer::onDetach() {
    m_Window->destroyImGuiContext();
}
