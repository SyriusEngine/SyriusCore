#include "VertexArrayLayer.hpp"

VertexArrayLayer::VertexArrayLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                                   EasyIni::Configuration &config) : Layer(context, window, config) {
    m_ShaderProgram = m_ShaderLibrary.loadShader("VertexBuffer");

    m_VertexLayout = m_Context->createVertexLayout();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("TexCoord", SR_FLOAT32_2);

    Mesh rectangle = createRectangle();

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    vboDesc.data = rectangle.vertices.data();
    vboDesc.layout = m_VertexLayout;
    vboDesc.count = rectangle.vertices.size();
    m_VertexBuffer = m_Context->createVertexBuffer(vboDesc);

    IndexBufferDesc iboDesc;
    iboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    iboDesc.data = rectangle.indices.data();
    iboDesc.count = rectangle.indices.size();
    iboDesc.dataType = SR_UINT32;
    m_IndexBuffer = m_Context->createIndexBuffer(iboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = m_ShaderProgram.vertexShader;
    vaoDesc.vertexBuffer = m_VertexBuffer;
    vaoDesc.indexBuffer = m_IndexBuffer;
    m_VertexArray = m_Context->createVertexArray(vaoDesc);

    addImGuiDrawFunction([this]{
        Layer::imGuiIndexBufferPanel(m_IndexBuffer);
        Layer::imGuiVertexArrayPanel(m_VertexArray);
    });
}

VertexArrayLayer::~VertexArrayLayer() = default;

void VertexArrayLayer::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();
}

void VertexArrayLayer::onEvent(const Event &event) {

}
