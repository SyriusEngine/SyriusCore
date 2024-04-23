#include "ConstantBufferLayer.hpp"

ConstantBufferLayer::ConstantBufferLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                                         EasyIni::Configuration &config) : Layer(context, window, config){
    m_ShaderProgram = m_ShaderLibrary.loadShader("ConstantBuffer");

    Mesh rectangle = createRectangle();
    m_VertexArray = loadMesh(rectangle, m_ShaderProgram);

    glm::mat4 transform = glm::mat4(1.0f);
    ConstantBufferDesc transformDesc;
    transformDesc.data = &transform;
    transformDesc.size = sizeof(glm::mat4);
    transformDesc.shaderStage = SR_SHADER_VERTEX;
    transformDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    transformDesc.name = "TransformData";
    m_TransformBuffer = m_Context->createConstantBuffer(transformDesc);

    ConstantBufferDesc colorDataDesc;
    colorDataDesc.data = &m_ColorData;
    colorDataDesc.size = sizeof(ColorData);
    colorDataDesc.shaderStage = SR_SHADER_FRAGMENT;
    colorDataDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    colorDataDesc.name = "ColorData";
    m_ColorDataBuffer = m_Context->createConstantBuffer(colorDataDesc);

    addImGuiDrawFunction([this]{
        Layer::imGuiRenderTransformConstantBuffer(m_TransformBuffer);
        imGuiRenderColorDataConstantBuffer(m_ColorDataBuffer);
    });
}

ConstantBufferLayer::~ConstantBufferLayer() = default;

void ConstantBufferLayer::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_TransformBuffer->bind(0);
    m_ColorDataBuffer->bind(1);
    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();

}

void ConstantBufferLayer::onEvent(const Event &event) {

}

void ConstantBufferLayer::imGuiRenderColorDataConstantBuffer(ResourceView<ConstantBuffer>& constantBuffer) {
    ImGui::Begin("Color Data Constant Buffer");
    if (ImGui::ColorEdit4("Color", &m_ColorData.color.x)){
        m_ColorDataBuffer->setData(&m_ColorData, sizeof(ColorData));
    }
    if (ImGui::SliderFloat("Lerp Factor", &m_ColorData.lerpFactor.x, 0.0f, 1.0f)){
        m_ColorDataBuffer->setData(&m_ColorData, sizeof(ColorData));
    }
    ImGui::End();
}
