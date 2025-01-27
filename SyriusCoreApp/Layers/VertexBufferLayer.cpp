#include "VertexBufferLayer.hpp"

VertexBufferLayer::VertexBufferLayer(ResourceView<Context> &context,
                                     const UP<SyriusWindow> &window,
                                     EasyIni::Configuration &config) :
Layer(context, window, config){
    m_ShaderProgram = m_ShaderLibrary.loadShader("VertexBuffer");

    m_VertexLayout = m_Context->createVertexLayout();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
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
        imGuiVertexBufferPanel(m_VertexBuffer);
    });
}

VertexBufferLayer::~VertexBufferLayer() = default;

void VertexBufferLayer::onUpdate() {
    Layer::onUpdate();

    m_Context->clear();
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();

}

void VertexBufferLayer::onEvent(const Event &event) {

}

void VertexBufferLayer::imGuiVertexBufferPanel(ResourceView<VertexBuffer> & vertexBuffer) {
    ImGui::Begin("Vertex Buffer Panel");

    static bool useRectangle = true;
    if (ImGui::Checkbox("Draw Rectangle", &useRectangle)){
        if (!useRectangle){
            vertexBuffer->setData(s_Triangle, 3);
        }
        else{
            vertexBuffer->setData(s_Rectangle, 6);
        }
    }
    if (ImGui::Button("Read Data")){
        auto pData = vertexBuffer->getData();
        auto floatData = reinterpret_cast<Vertex*>(pData.get());
        for(int i = 0; i < vertexBuffer->getCount(); i++){
            auto& vertex = floatData[i];
            std::cout << "Pos: " << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z;
            std::cout << ", Color: " << vertex.color.x << ", " << vertex.color.y << ", " << vertex.color.z;
            std::cout << ", Normal: " << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z;
            std::cout << ", Tangent: " << vertex.tangent.x << ", " << vertex.tangent.y << ", " << vertex.tangent.z;
            std::cout << ", TexCoord: " << vertex.texCoord.x << ", " << vertex.texCoord.y << std::endl;
        }

    }

    ImGui::End();
}
