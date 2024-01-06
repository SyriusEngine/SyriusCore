#include "TestIndexBuffer.hpp"

TestIndexBuffer::TestIndexBuffer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                                 EasyIni::Configuration &config) : Layer(context, window, config), m_TestShaderLibrary("./Resources/TestShaders", context) {
    m_ShaderProgram = m_TestShaderLibrary.loadShader("VertexBuffer");

    m_VertexLayout = m_Context->createVertexDescription();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);

    const float rectangle[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   //0
            0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,    //1
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,    //2
            0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,    //3
    };

    const uint32_t indices[] = {
            0, 1, 2,
            1, 2, 3
    };

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DEFAULT;
    vboDesc.data = rectangle;
    vboDesc.layout = m_VertexLayout;
    vboDesc.count = 4;
    m_VertexBuffer = m_Context->createVertexBuffer(vboDesc);

    IndexBufferDesc iboDesc;
    iboDesc.usage = SR_BUFFER_USAGE_DEFAULT;
    iboDesc.data = indices;
    iboDesc.count = 6;
    iboDesc.dataType = SR_UINT32;
    m_IndexBuffer = m_Context->createIndexBuffer(iboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = m_ShaderProgram.vertexShader;
    vaoDesc.vertexBuffer = m_VertexBuffer;
    vaoDesc.indexBuffer = m_IndexBuffer;
    m_VertexArray = m_Context->createVertexArray(vaoDesc);
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
