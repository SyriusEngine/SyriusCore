#include "MeshComponent.hpp"
#include "ShaderComponent.hpp"

#include "../Utils.hpp"
#include "../Core/ComponentContainer.hpp"

MeshComponent::MeshComponent(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer* container):
IComponent(window, context, container){
    ConstantBufferDesc transformDesc;
    transformDesc.data = &m_Transform;
    transformDesc.size = sizeof(glm::mat4);
    transformDesc.shaderStage = SR_SHADER_VERTEX;
    transformDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    transformDesc.name = "TransformData";
    m_TransformBuffer = m_Context->createConstantBuffer(transformDesc);

    m_VertexLayout = m_Context->createVertexLayout();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("TexCoord", SR_FLOAT32_2);

    const MeshData cube = createCube();
    setMesh(cube);
}

void MeshComponent::onUpdate(Duration deltaTime){
    m_TransformBuffer->bindShaderResource(0);
    m_Context->draw(m_VertexArray);
}

void MeshComponent::onImGui(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "Mesh");
    ImGui::Text("Vertex Count: %lu", m_VertexCount);
    ImGui::Text("Index Count: %lu", m_IndexCount);
    ImGui::Text("Draw Mode: %s", drawModeToString(m_VertexArray->getDrawMode()).c_str());

    imGuiDrawVertexArray();
    imGuiDrawMeshSelector();
    imGuiDrawTransform();

    imGuiEnd(windowData);
}

void MeshComponent::imGuiDrawTransform() {
    if (ImGui::DragFloat3("Translation", glm::value_ptr(m_Translate), 0.01f)){
        setTransformData();
    }
    if (ImGui::DragFloat3("Rotation", glm::value_ptr(m_Rotate), 0.01f)){
        setTransformData();
    }
    if (ImGui::DragFloat3("Scale", glm::value_ptr(m_Scale), 0.1f)){
        setTransformData();
    }
}

void MeshComponent::imGuiDrawVertexArray() {
    static std::vector<std::string> drawModes = {"SR_DRAW_POINTS", "SR_DRAW_LINES", "SR_DRAW_LINE_STRIP",
                                                "SR_DRAW_TRIANGLES", "SR_DRAW_TRIANGLE_STRIP"};
    static int drawModeIndex = 3;
    if (ImGui::BeginCombo("Draw Mode", drawModes[drawModeIndex].c_str())){
        for(int i = 0; i < drawModes.size(); i++){
            bool isSelected = (drawModeIndex == i);
            if (ImGui::Selectable(drawModes[i].c_str(), isSelected)){
                drawModeIndex = i;
                m_CurrentDrawMode = static_cast<SR_DRAW_MODE>(i + 1);
                m_VertexArray->setDrawMode(m_CurrentDrawMode);
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void MeshComponent::imGuiDrawMeshSelector() {
    static std::vector<std::string> meshes = {"Cube", "Sphere"};
    static int meshIndex = 0;
    const int current = meshIndex;
    if (ImGui::BeginCombo("Mesh", meshes[meshIndex].c_str())){
        for(int i = 0; i < meshes.size(); i++){
            const bool isSelected = (meshIndex == i);
            if (ImGui::Selectable(meshes[i].c_str(), isSelected)){
                meshIndex = i;
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (meshIndex != current) {
        switch (meshIndex) {
            case 0:     setMesh(createCube());  break;
            case 1:     setMesh(createSphere(32, 32)); break;
            default: break;
        }
    }
}

void MeshComponent::setTransformData() const {
    const glm::mat4 translate  = glm::translate(glm::mat4(1.0f), m_Translate);
    const glm::mat4 rotate = glm::toMat4(glm::quat(m_Rotate));
    const glm::mat4 scale = glm::scale(m_Scale);

    Transform transform;
    transform.model = translate * scale * rotate;
    m_TransformBuffer->setData(&transform, sizeof(Transform));
}

void MeshComponent::setMesh(const MeshData &meshData) {
    const auto shader = m_Container->getComponent<ShaderComponent>();

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    vboDesc.data = meshData.vertices.data();
    vboDesc.layout = m_VertexLayout;
    vboDesc.count = meshData.vertices.size();
    auto vbo = m_Context->createVertexBuffer(vboDesc);

    IndexBufferDesc iboDesc;
    iboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    iboDesc.data = meshData.indices.data();
    iboDesc.count = meshData.indices.size();
    iboDesc.dataType = SR_UINT32;
    auto ibo = m_Context->createIndexBuffer(iboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = m_CurrentDrawMode;
    vaoDesc.vertexShader = shader->getProgram().vertexShader;
    vaoDesc.vertexBuffer = vbo;
    vaoDesc.indexBuffer = ibo;
    m_VertexArray = m_Context->createVertexArray(vaoDesc);
    m_VertexCount = vbo->getCount();
    m_IndexCount = ibo->getCount();
}

