#include "TestVertexBuffer.hpp"

const float s_Triangle[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
};

const float s_Rectangle[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};

TestVertexBuffer::TestVertexBuffer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                                   EasyIni::Configuration &config) : Layer(context, window, config),
                                   m_TestShaderLibrary("./Resources/TestShaders", context),
                                   m_DrawRectangle(true){
    m_Window->createImGuiContext();

    m_ShaderProgram = m_TestShaderLibrary.loadShader("VertexBuffer");

    m_VertexLayout = m_Context->createVertexLayout();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("TexCoord", SR_FLOAT32_2);


    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    vboDesc.data = s_Rectangle;
    vboDesc.layout = m_VertexLayout;
    vboDesc.count = 6;
    m_VertexBuffer = m_Context->createVertexBuffer(vboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = m_ShaderProgram.vertexShader;
    vaoDesc.vertexBuffer = m_VertexBuffer;
    m_VertexArray = m_Context->createVertexArray(vaoDesc);
}

TestVertexBuffer::~TestVertexBuffer() {
    m_Window->destroyImGuiContext();
}

void TestVertexBuffer::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);

    m_Window->onImGuiBegin();

    ImGui::Begin("Test Vertex Buffer");
    if (ImGui::Checkbox("Draw Rectangle", &m_DrawRectangle)){
        if (!m_DrawRectangle){
            m_VertexBuffer->setData(s_Triangle, 3);
        }
        else{
            m_VertexBuffer->setData(s_Rectangle, 6);
        }

    }
    ImGui::End();
    m_Window->onImGuiEnd();

    m_Context->endRenderPass();
}

void TestVertexBuffer::onEvent(const Event &event) {

}
