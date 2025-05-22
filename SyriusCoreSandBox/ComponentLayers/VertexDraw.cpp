#include "VertexDraw.hpp"

static const Vertex s_VertexDrawRectangle[] = {
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f),
    glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f),
    glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f),

    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f),
    glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f),
    glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f),
};

static const Vertex s_VertexDrawTriangle[] = {
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
    glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.5f, 1.0f),
    glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f),
};

VertexDraw::VertexDraw(const UP<SyriusWindow> &window, const ResourceView<Context> &context):
IComponent(window, context){
    m_ShaderProgram = loadShader(m_Context, "VertexBuffer", "VertexBuffer");

    m_VertexLayout = m_Context->createVertexLayout();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("TexCoord", SR_FLOAT32_2);

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    vboDesc.data = s_VertexDrawRectangle;
    vboDesc.layout = m_VertexLayout;
    vboDesc.count = 6;
    m_VertexBuffer = m_Context->createVertexBuffer(vboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = m_ShaderProgram.vertexShader;
    vaoDesc.vertexBuffer = m_VertexBuffer;
    m_VertexArray = m_Context->createVertexArray(vaoDesc);
}

void VertexDraw::onUpdate(Duration deltaTime){
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);

    m_Context->endRenderPass();
}

void VertexDraw::onImGui(ImGuiWindowData &windowData){
    imGuiVertexBufferPanel(windowData);
    imGuiVertexArrayPanel(windowData);
}

void VertexDraw::imGuiVertexBufferPanel(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "VertexDraw");

    ImGui::Text("Mesh drawn without index buffer.");
    static bool useRectangle = true;
    if (ImGui::Checkbox("Draw Rectangle", &useRectangle)){
        if (!useRectangle){
            m_VertexBuffer->setData(s_VertexDrawTriangle, 3);
        }
        else{
            m_VertexBuffer->setData(s_VertexDrawRectangle, 6);
        }
    }

    static std::string bufferData;
    if (ImGui::Button("Read Data")) {
        bufferData.clear();
        auto pData = m_VertexBuffer->getData();
        auto vertexData = reinterpret_cast<Vertex*>(pData.get());
        for (Size i = 0; i < m_VertexBuffer->getCount(); i++) {
            Vertex& v = vertexData[i];
            bufferData += "Position: (" + std::to_string(v.position.x) + ", " + std::to_string(v.position.y) + ", " + std::to_string(v.position.z) + "), ";
            bufferData += "Color: (" + std::to_string(v.color.x) + ", " + std::to_string(v.color.y) + ", " + std::to_string(v.color.z) + "), ";
            bufferData += "Normal: (" + std::to_string(v.normal.x) + ", " + std::to_string(v.normal.y) + ", " + std::to_string(v.normal.z) + "), ";
            bufferData += "Tangent: (" + std::to_string(v.tangent.x) + ", " + std::to_string(v.tangent.y) + ", " + std::to_string(v.tangent.z) + "), ";
            bufferData += "TexCoord: (" + std::to_string(v.texCoord.x) + ", " + std::to_string(v.texCoord.y) + ")\n";
        }
    }
    ImGui::TextUnformatted(bufferData.c_str());

    imGuiEnd(windowData);
}

void VertexDraw::imGuiVertexArrayPanel(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "VertexArray1");

    static std::vector<std::string> drawModes = {"SR_DRAW_POINTS", "SR_DRAW_LINES", "SR_DRAW_LINE_STRIP",
                                                "SR_DRAW_TRIANGLES", "SR_DRAW_TRIANGLE_STRIP"};
    static int drawModeIndex = 3;
    auto drawMode = m_VertexArray->getDrawMode();
    ImGui::Text("Draw Mode: %s", drawModeToString(drawMode).c_str());
    if (ImGui::BeginCombo("Draw Mode", drawModes[drawModeIndex].c_str())){
        for(int i = 0; i < drawModes.size(); i++){
            bool isSelected = (drawModeIndex == i);
            if (ImGui::Selectable(drawModes[i].c_str(), isSelected)){
                drawModeIndex = i;
                m_VertexArray->setDrawMode(static_cast<SR_DRAW_MODE>(i + 1));
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    imGuiEnd(windowData);
}


