#include "ShaderStorageBufferLayer.hpp"

#include "../Components/ShaderComponent.hpp"
#include "../Core/ComponentContainer.hpp"

ShaderStorageBufferLayer::ShaderStorageBufferLayer(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    auto shader = container->getComponent<ShaderComponent>();
    shader->setShader("ShaderStorageBuffer", "Texture");

    constexpr i32 border = 16;
    constexpr float spacing = 4.0f;
    for (int x = -border; x < border; x++) {
        for (int y = -border; y < border; y++) {
            for (int z = -border; z < border; z++) {
                Transform transform;
                transform.model = glm::translate(glm::vec3(x * spacing, y * spacing, z * spacing));
                m_Transforms.push_back(transform);
                m_InstanceCount++;
            }
        }
    }

    ShaderStorageBufferDesc transformDesc;
    transformDesc.data = m_Transforms.data();
    transformDesc.size = sizeof(Transform) * m_Transforms.size();
    transformDesc.shaderStage = SR_SHADER_VERTEX;
    transformDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    transformDesc.name = "TransformData";
    m_TransformBuffer = m_Context->createShaderStorageBuffer(transformDesc);

    m_VertexLayout = m_Context->createVertexLayout();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("TexCoord", SR_FLOAT32_2);

    const MeshData cube = createCube();
    setMesh(cube);
}

void ShaderStorageBufferLayer::onUpdate(Duration deltaTime){
    m_TransformBuffer->bindShaderResource(0);
    m_Context->drawInstanced(m_VertexArray, m_InstanceCount);
}

void ShaderStorageBufferLayer::onImGui(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "Mesh");
    ImGui::Text("Vertex Count: %lu", m_VertexCount);
    ImGui::Text("Index Count: %lu", m_IndexCount);
    ImGui::Text("Instance Count: %lu", m_InstanceCount);
    ImGui::Text("Draw Mode: %s", drawModeToString(m_VertexArray->getDrawMode()).c_str());

    imGuiDrawVertexArray();
    imGuiDrawMeshSelector();
    imGuiDrawTransform();

    imGuiEnd(windowData);
}

void ShaderStorageBufferLayer::imGuiDrawTransform() {
    ImGui::TextWrapped("When the correct transformation is filled in, press Upload. We"
                       "do not want to send data to the GPU every time a slider changes."
                       "This causes huge amount of pipeline stalls since we force the driver to synchronize");
    ImGui::DragFloat3("Translation", glm::value_ptr(m_Translate), 0.01f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(m_Rotate), 0.01f);
    ImGui::DragFloat3("Scale", glm::value_ptr(m_Scale), 0.1f);
    if (ImGui::Button("Upload")) {
        setTransformData();
    }
}

void ShaderStorageBufferLayer::imGuiDrawVertexArray() {
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

void ShaderStorageBufferLayer::imGuiDrawMeshSelector() {
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

void ShaderStorageBufferLayer::setTransformData() {
    const glm::mat4 translate  = glm::translate(glm::mat4(1.0f), m_Translate);
    const glm::mat4 rotate = glm::toMat4(glm::quat(m_Rotate));
    const glm::mat4 scale = glm::scale(m_Scale);
    const glm::mat4 result = translate * scale * rotate;
    for (auto& t: m_Transforms) {
        t.model = result;
    }
    m_TransformBuffer->setData(m_Transforms.data(), m_Transforms.size() * sizeof(Transform));
}

void ShaderStorageBufferLayer::setMesh(const MeshData &meshData) {
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

