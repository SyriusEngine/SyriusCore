#include "TestVertexBuffer.hpp"

TestVertexBuffer::TestVertexBuffer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                                   EasyIni::Configuration &config) : Layer(context, window, config), m_TestShaderLibrary("./Resources/TestShaders", context) {
    m_ShaderProgram = m_TestShaderLibrary.loadShader("VertexBuffer");

    m_VertexLayout = m_Context->createVertexDescription();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);

    const float triangle[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DEFAULT;
    vboDesc.data = triangle;
    vboDesc.layout = m_VertexLayout;
    vboDesc.count = 3;
    m_VertexBuffer = m_Context->createVertexBuffer(vboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = m_ShaderProgram.vertexShader;
    vaoDesc.vertexBuffer = m_VertexBuffer;
    m_VertexArray = m_Context->createVertexArray(vaoDesc);
}

TestVertexBuffer::~TestVertexBuffer() {

}

void TestVertexBuffer::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);

    m_Context->endRenderPass();
}

void TestVertexBuffer::onEvent(const Event &event) {

}
