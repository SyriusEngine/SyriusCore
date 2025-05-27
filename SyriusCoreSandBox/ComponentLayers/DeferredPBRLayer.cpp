#include "DeferredPBRLayer.hpp"

#include "../Components/ShaderComponent.hpp"
#include "../Core/ComponentContainer.hpp"

DeferredPBRLayer::DeferredPBRLayer(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container),
m_PrivContainer(createUP<ComponentContainer>(window, context)){
    m_PrivContainer->createComponent("PBR");

    auto fbLayout = m_Context->createFrameBufferLayout();
    ViewportDesc vpDesc;
    vpDesc.width = m_Context->getWidth();
    vpDesc.height = m_Context->getHeight();
    fbLayout->addViewportDesc(vpDesc);
    ColorAttachmentDesc f32ca;
    f32ca.format = SR_TEXTURE_RGBA_F32;
    f32ca.width = m_Context->getWidth();
    f32ca.height = m_Context->getHeight();
    f32ca.clearColor[0] = 1.0f;
    fbLayout->addColorAttachmentDesc(f32ca); // Position
    fbLayout->addColorAttachmentDesc(f32ca); // Normal
    fbLayout->addColorAttachmentDesc(f32ca); // Albedo
    fbLayout->addColorAttachmentDesc(f32ca); // mrao
    DepthStencilAttachmentDesc dsDesc;
    dsDesc.format = SR_TEXTURE_DEPTH_24_STENCIL_8;
    dsDesc.width = m_Context->getWidth();
    dsDesc.height = m_Context->getHeight();
    dsDesc.clearDepth = 1.0f;
    dsDesc.enableDepthTest = m_EnableDepthTest;
    fbLayout->addDepthStencilAttachmentDesc(dsDesc);
    m_GBuffer = m_Context->createFrameBuffer(fbLayout);

    // 2nd Stage: Draw the GBuffer to a rectangle which will contain the light info
    m_ScreenProgram = loadShader(m_Context, "LightPass", "LightPass");

    const auto rectangle = createRectangle(1.0f);
    m_ScreenVertexArray = loadMesh(m_Context, rectangle, m_ScreenProgram);

    // By default, create 1 light
    m_LightData.positions[0] = glm::vec4(0.0f, 3.0f, 0.0f, 1.0f);
    m_LightData.colors[0] = glm::vec4(255.0f, 255.0f, 255.0f, 1.0f);
    m_LightData.count = glm::uvec4(1, 0, 0, 0);
    ConstantBufferDesc lightDesc;
    lightDesc.data = &m_LightData;
    lightDesc.size = sizeof(LightData);
    lightDesc.count = 1;
    lightDesc.shaderStage = SR_SHADER_FRAGMENT;
    lightDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    lightDesc.name = "LightData";
    m_LightBuffer = m_Context->createConstantBuffer(lightDesc);
}

void DeferredPBRLayer::onUpdate(const Duration deltaTime) {
    m_GBuffer->clear();
    m_Context->beginRenderPass(m_GBuffer);
    m_PrivContainer->onUpdate(deltaTime);
    m_Context->endRenderPass(m_GBuffer);

    m_Context->beginRenderPass();
    m_ScreenProgram.shaderProgram->bind();
    m_LightBuffer->bindShaderResource(5);
    m_GBuffer->getColorAttachment(0)->bindShaderResource(0);
    m_GBuffer->getColorAttachment(1)->bindShaderResource(1);
    m_GBuffer->getColorAttachment(2)->bindShaderResource(2);
    m_GBuffer->getColorAttachment(3)->bindShaderResource(3);
    m_Context->draw(m_ScreenVertexArray);
}

void DeferredPBRLayer::onEvent(const Event &event) {
    m_PrivContainer->onEvent(event);
}

void DeferredPBRLayer::onImGui(ImGuiWindowData &windowData) {
    m_PrivContainer->onImGui(windowData);
    imGuiFrameBuffer(windowData);
}

void DeferredPBRLayer::imGuiFrameBuffer(ImGuiWindowData &windowData) {
    imGuiBegin(windowData, "GBuffer");
    if (ImGui::Checkbox("Depth Test", &m_EnableDepthTest)){
        m_GBuffer->enableDepthTest(m_EnableDepthTest);
    }

    imGuiEnd(windowData);
}
