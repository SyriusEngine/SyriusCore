#include "FrameBufferLayer.hpp"

#include "../Components/ShaderComponent.hpp"
#include "../Core/ComponentContainer.hpp"

FrameBufferLayer::FrameBufferLayer(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container),
m_PrivContainer(createUP<ComponentContainer>(window, context)){
    m_PrivContainer->createComponent("MVPTextured");

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
    fbLayout->addColorAttachmentDesc(f32ca); // first color attachment
    fbLayout->addColorAttachmentDesc(f32ca); // second color attachment
    DepthStencilAttachmentDesc dsDesc;
    dsDesc.format = SR_TEXTURE_DEPTH_24_STENCIL_8;
    dsDesc.width = m_Context->getWidth();
    dsDesc.height = m_Context->getHeight();
    dsDesc.enableShaderAccess = true;
    dsDesc.enableDepthTest = m_EnableDepthTest;
    fbLayout->addDepthStencilAttachmentDesc(dsDesc);

    m_FrameBuffer = m_Context->createFrameBuffer(fbLayout);

    // 2nd Stage: Draw the framebuffer to a rectangle
    m_ScreenProgram = loadShader(m_Context, "Screen", "Screen");

    auto [vertices, indices] = createRectangle(0.75);

    auto layout = m_Context->createVertexLayout();
    layout->addAttribute("Position", SR_FLOAT32_3);
    layout->addAttribute("Color", SR_FLOAT32_3);
    layout->addAttribute("Normal", SR_FLOAT32_3);
    layout->addAttribute("Tangent", SR_FLOAT32_3);
    layout->addAttribute("TexCoord", SR_FLOAT32_2);

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    vboDesc.data = vertices.data();
    vboDesc.layout = layout;
    vboDesc.count = vertices.size();
    auto vbo = m_Context->createVertexBuffer(vboDesc);

    IndexBufferDesc iboDesc;
    iboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    iboDesc.data = indices.data();
    iboDesc.count = indices.size();
    iboDesc.dataType = SR_UINT32;
    auto ibo = m_Context->createIndexBuffer(iboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = m_ScreenProgram.vertexShader;
    vaoDesc.vertexBuffer = vbo;
    vaoDesc.indexBuffer = ibo;
    m_ScreenVertexArray = m_Context->createVertexArray(vaoDesc);
}

void FrameBufferLayer::onUpdate(const Duration deltaTime) {
    m_FrameBuffer->clear();
    m_Context->beginRenderPass(m_FrameBuffer);
    m_PrivContainer->onUpdate(deltaTime);
    m_Context->endRenderPass(m_FrameBuffer);

    m_Context->beginRenderPass();
    switch (m_SelectedTexture) {
        case 0: m_FrameBuffer->getColorAttachment(0)->bindShaderResource(0); break;
        case 1: m_FrameBuffer->getColorAttachment(1)->bindShaderResource(0); break;
        case 2: m_FrameBuffer->getDepthStencilAttachment()->bindShaderResource(0); break;
        default:
            break;
    }
    m_ScreenProgram.shaderProgram->bind();
    m_Context->draw(m_ScreenVertexArray);
}

void FrameBufferLayer::onEvent(const Event &event) {
    m_PrivContainer->onEvent(event);
}

void FrameBufferLayer::onImGui(ImGuiWindowData &windowData) {
    m_PrivContainer->onImGui(windowData);
    imGuiFrameBuffer(windowData);
}

void FrameBufferLayer::imGuiFrameBuffer(ImGuiWindowData &windowData) {
    static std::vector<std::string> sampleFrom = {"Normal Color", "Inverted Color", "Depth Stencil Attachment"};

    imGuiBegin(windowData, "FrameBuffer");
    if (ImGui::Checkbox("Depth Test", &m_EnableDepthTest)){
        m_FrameBuffer->enableDepthTest(m_EnableDepthTest);
    }
    if (ImGui::BeginCombo("Attachment", sampleFrom[m_SelectedTexture].c_str())){
        for(int i = 0; i < sampleFrom.size(); i++){
            bool isSelected = (m_SelectedTexture == i);
            if (ImGui::Selectable(sampleFrom[i].c_str(), isSelected)){
                m_SelectedTexture = i;
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    imGuiEnd(windowData);
}





