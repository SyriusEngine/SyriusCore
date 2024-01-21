#include "TestTexture.hpp"

TestTexture::TestTexture(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config),
m_TestShaderLibrary("./Resources/TestShaders", context),
m_UseHdrTexture(false){
    m_ShaderProgram = m_TestShaderLibrary.loadShader("Texture");

    Mesh rectangle = createRectangle();
    m_VertexArray = loadMesh(rectangle, m_ShaderProgram);

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    ImageFileDesc imgDesc;
    imgDesc.fileName = "./Resources/Textures/awesomeface.png";
    imgDesc.flipOnAccess = true;
    auto img = createImage(imgDesc);
    Texture2DImageDesc texDesc;
    texDesc.image = createResourceView(img);
    m_Texture = m_Context->createTexture2D(texDesc);

    m_Window->createImGuiContext();
}

TestTexture::~TestTexture() {
    m_Window->destroyImGuiContext();
}

void TestTexture::onUpdate() {
    onRender();
    onImGuiRender();
}

void TestTexture::onEvent(const Event &event) {

}

void TestTexture::onRender() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Texture->bind(0);
    m_Sampler->bind(0);
    m_Context->draw(m_VertexArray);

    m_Context->endRenderPass();
}

void TestTexture::onImGuiRender() {
    m_Window->onImGuiBegin();
    ImGui::Begin("Texture Controls");
    ImGui::Checkbox("Use HDR Texture", &m_UseHdrTexture);
    ImGui::End();

    m_Window->onImGuiEnd();
}
