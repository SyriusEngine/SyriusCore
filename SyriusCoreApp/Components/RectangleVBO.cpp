#include "RectangleVBO.hpp"

RectangleVBO::RectangleVBO(UP<SyriusWindow>& window, ResourceView<Context>& context) :
AppComponent(window, context) {
    m_ShaderProgram = loadShader("VertexBuffer");

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
    m_Rectangle = m_Context->createVertexBuffer(vboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = m_ShaderProgram.vertexShader;
    vaoDesc.vertexBuffer = m_Rectangle;
    m_VertexArray = m_Context->createVertexArray(vaoDesc);
}

void RectangleVBO::onRender() {
    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);
}

void RectangleVBO::onImGuiRender(ImGuiSizeData &sizeData) {
    AppComponent::imGuiBeginPanel(sizeData, "Rectangle VBO");

    static bool useRectangle = true;
    if (ImGui::Checkbox("Draw Rectangle", &useRectangle)){
        if (!useRectangle){
            m_Rectangle->setData(s_Triangle, 3);
        }
        else{
            m_Rectangle->setData(s_Rectangle, 6);
        }
    }
    if (ImGui::Button("Read Data")){
        auto pData = m_Rectangle->getData();
        auto floatData = reinterpret_cast<Vertex*>(pData.get());
        for(int i = 0; i < m_Rectangle->getCount(); i++){
            auto& vertex = floatData[i];
            std::cout << "Pos: " << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z;
            std::cout << ", Color: " << vertex.color.x << ", " << vertex.color.y << ", " << vertex.color.z;
            std::cout << ", Normal: " << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z;
            std::cout << ", Tangent: " << vertex.tangent.x << ", " << vertex.tangent.y << ", " << vertex.tangent.z;
            std::cout << ", TexCoord: " << vertex.texCoord.x << ", " << vertex.texCoord.y << std::endl;
        }

    }

    AppComponent::imGuiEndPanel(sizeData);
}
