#include "TestShaderStorageBuffer.hpp"

const float s_Triangle[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
};

const float s_ZeroPosition[] = { // default SSBO data
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
};

TestShaderStorageBuffer::TestShaderStorageBuffer(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config),
m_TestShaderLibrary("./Resources/TestShaders", context){
    m_Window->createImGuiContext();

    m_ShaderProgram = m_TestShaderLibrary.loadShader("ShaderStorageBuffer");

    m_VertexLayout = m_Context->createVertexLayout();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("TexCoord", SR_FLOAT32_2);

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    vboDesc.data = s_Triangle;
    vboDesc.layout = m_VertexLayout;
    vboDesc.count = 3;
    m_VertexBuffer = m_Context->createVertexBuffer(vboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = m_ShaderProgram.vertexShader;
    vaoDesc.vertexBuffer = m_VertexBuffer;
    m_VertexArray = m_Context->createVertexArray(vaoDesc);

    ShaderStorageBufferDesc ssboDesc;
    ssboDesc.data = s_ZeroPosition;
    ssboDesc.size = sizeof(s_ZeroPosition);
    ssboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    ssboDesc.name = "PositionData";
    m_SSBO = m_Context->createShaderStorageBuffer(ssboDesc);
}

TestShaderStorageBuffer::~TestShaderStorageBuffer() {
    m_Window->destroyImGuiContext();
}

void TestShaderStorageBuffer::onUpdate() {
    m_Context->beginRenderPass();
    render();
    renderImGui();

    m_Context->endRenderPass();
}

void TestShaderStorageBuffer::onEvent(const Event &event) {

}

void TestShaderStorageBuffer::render() {
    m_ShaderProgram.shaderProgram->bind();
    m_SSBO->bindShaderResource(0);
    m_Context->draw(m_VertexArray);
}

void TestShaderStorageBuffer::renderImGui() {
    m_Window->onImGuiBegin();

    ImGui::Begin("Test Shader Storage Buffer");
    ImGui::TextWrapped("This test draws a simple triangle. When the button read SSBO is pressed, the vertex shader SSBO is read back into CPU RAM and printed in the console.");
    ImGui::NewLine();
    ImGui::TextWrapped("The expected data are the vertex positions of the triangle, so 3 float3 values.");

    if (ImGui::Button("Read SSBO")) {
        auto resource = m_SSBO->getData();
        auto pData = reinterpret_cast<float*>(resource.get());
        for (int i = 0; i < 9; i += 4) {
            std::cout << "Vertex " << i / 3 << ": " << pData[i] << ", " << pData[i + 1] << ", " << pData[i + 2] << std::endl;
        }

    }

    ImGui::End();

    m_Window->onImGuiEnd();
}
