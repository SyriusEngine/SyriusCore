#include "IndexDraw.hpp"

IndexDraw::IndexDraw(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer* container):
IComponent(window, context, container){
    m_ShaderProgram = loadShader(m_Context, "VertexBuffer", "VertexBuffer");

    m_VertexLayout = m_Context->createVertexLayout();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("TexCoord", SR_FLOAT32_2);

    MeshData rectangle = createRectangle();

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    vboDesc.data = rectangle.vertices.data();
    vboDesc.layout = m_VertexLayout;
    vboDesc.count = 6;
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
}

void IndexDraw::onUpdate(Duration deltaTime){
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);

    m_Context->endRenderPass();
}

void IndexDraw::onImGui(ImGuiWindowData &windowData) {
    imGuiVertexBufferPanel(windowData);
    imGuiIndexBufferPanel(windowData);
    imGuiVertexArrayPanel(windowData);
}

void IndexDraw::imGuiVertexBufferPanel(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "VertexBuffer1");

    ImGui::Text("Count: %d", m_VertexBuffer->getCount());
    ImGui::Text("Size: %d", m_VertexBuffer->getSize());
    ImGui::TextWrapped("Drag this slider to change the size of the rectangle, this will update the Vertices itself");

    if (ImGui::SliderFloat("Size", &m_Size, 0.0f, 1.0f)) {
        auto [vertices, _] = createRectangle(m_Size);
        m_VertexBuffer->setData(vertices.data(), vertices.size());
    }
    imGuiEnd(windowData);
}

void IndexDraw::imGuiIndexBufferPanel(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "IndexBuffer1");

    ImGui::Text("Count: %d", m_IndexBuffer->getCount());
    ImGui::Text("Size: %d", m_IndexBuffer->getSize());
    static bool s_DrawRectangle = true;
    if (ImGui::Checkbox("Draw Rectangle", &s_DrawRectangle)) {
        if (s_DrawRectangle) {
            auto [_, indices] = createRectangle(m_Size);
            m_IndexBuffer->setData(indices.data(), indices.size());
        }
        else {
            auto [_, indices] = createTriangle(m_Size);
            m_IndexBuffer->setData(indices.data(), indices.size());
        }
    }
    imGuiEnd(windowData);
}

void IndexDraw::imGuiVertexArrayPanel(ImGuiWindowData &windowData){
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
