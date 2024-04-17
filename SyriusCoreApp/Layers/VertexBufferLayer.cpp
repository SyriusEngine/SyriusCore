#include "VertexBufferLayer.hpp"

VertexBufferLayer::VertexBufferLayer(ResourceView<Context> &context,
                                     const Resource<SyriusWindow> &window,
                                     EasyIni::Configuration &config) :
Layer(context, window, config){
    m_ShaderProgram = m_ShaderLibrary.loadShader("VertexBuffer");

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

    addImGuiDrawFunction([this]{
        Layer::imGuiVertexBufferPanel(m_VertexBuffer);
    });
}

VertexBufferLayer::~VertexBufferLayer() {

}

void VertexBufferLayer::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();

}

void VertexBufferLayer::onEvent(const Event &event) {

}
