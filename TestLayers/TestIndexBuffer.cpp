#include "TestIndexBuffer.hpp"

TestIndexBuffer::TestIndexBuffer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                                 EasyIni::Configuration &config) : Layer(context, window, config), m_TestShaderLibrary("./Resources/TestShaders", context) {
    m_ShaderProgram = m_TestShaderLibrary.loadShader("VertexBuffer");

    Mesh rectangle = createRectangle();
    m_VertexArray = loadMesh(rectangle, m_ShaderProgram);
}

TestIndexBuffer::~TestIndexBuffer() {

}

void TestIndexBuffer::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);

    m_Context->endRenderPass();
}

void TestIndexBuffer::onEvent(const Event &event) {

}
