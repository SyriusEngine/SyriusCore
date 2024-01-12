#include "TestTexture.hpp"

TestTexture::TestTexture(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                         EasyIni::Configuration &config) : Layer(context, window, config), m_TestShaderLibrary("./Resources/TestShaders", context) {
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
}

TestTexture::~TestTexture() {

}

void TestTexture::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Texture->bind(0);
    m_Sampler->bind(0);
    m_Context->draw(m_VertexArray);

    m_Context->endRenderPass();
}

void TestTexture::onEvent(const Event &event) {

}
