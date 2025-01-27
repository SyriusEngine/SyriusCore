#include "RectangleVAO.hpp"

RectangleVAO::RectangleVAO(UP<SyriusWindow> &window, ResourceView<Context> &context) :
AppComponent(window, context) {
    m_ShaderProgram = loadShader("VertexBuffer");

    auto rectangleMesh = createRectangle();
    m_Rectangle = loadMesh(rectangleMesh, m_ShaderProgram, SR_BUFFER_USAGE_DYNAMIC);

}

void RectangleVAO::onRender() {
    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_Rectangle.vertexArray);
}

void RectangleVAO::onImGuiRender(ImGuiSizeData &sizeData) {
    imGuiRenderIndexBuffer(sizeData);
    imGuiRenderVertexArray(sizeData);
}

void RectangleVAO::imGuiRenderIndexBuffer(ImGuiSizeData &sizeData) {
    AppComponent::imGuiBeginPanel(sizeData, "Rectangle Index Buffer");

    static bool useRectangle = true;
    if (ImGui::Checkbox("Draw Rectangle", &useRectangle)){
        if (!useRectangle){
            Mesh triangle = createTriangle();
            m_Rectangle.indexBuffer->setData(triangle.indices.data(), triangle.indices.size());
        }
        else{
            Mesh rectangle = createRectangle();
            m_Rectangle.indexBuffer->setData(rectangle.indices.data(), rectangle.indices.size());
        }
    }
    if (ImGui::Button("Read Data")){
        auto pData = m_Rectangle.indexBuffer->getData();
        auto u32Data = reinterpret_cast<u32*>(pData.get());
        for(int i = 0; i < m_Rectangle.indexBuffer->getCount(); i++){
            std::cout << u32Data[i];
            if (i != m_Rectangle.indexBuffer->getCount() - 1){
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    AppComponent::imGuiEndPanel(sizeData);
}

void RectangleVAO::imGuiRenderVertexArray(ImGuiSizeData &sizeData) {
    AppComponent::imGuiBeginPanel(sizeData, "Rectangle Vertex Array");

    static std::array<std::string, 5> drawModes = {"SR_DRAW_POINTS", "SR_DRAW_LINES", "SR_DRAW_LINE_STRIP",
                                                 "SR_DRAW_TRIANGLES", "SR_DRAW_TRIANGLE_STRIP"};
    static int drawModeIndex = 3;
    auto drawMode = m_Rectangle.vertexArray->getDrawMode();
    ImGui::Text("Draw Mode: %s", drawModeToString(drawMode).c_str());
    if (ImGui::BeginCombo("Draw Mode", drawModes[drawModeIndex].c_str())){
        for(int i = 0; i < drawModes.size(); i++){
            bool isSelected = (drawModeIndex == i);
            if (ImGui::Selectable(drawModes[i].c_str(), isSelected)){
                drawModeIndex = i;
                m_Rectangle.vertexArray->setDrawMode(static_cast<SR_DRAW_MODE>(i + 1));
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    
    AppComponent::imGuiEndPanel(sizeData);
}
