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

    const auto shader = m_Container->getComponent<ShaderComponent>();

    m_VertexLayout = m_Context->createVertexLayout();
    m_VertexLayout->addAttribute("Position", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Color", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Normal", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("Tangent", SR_FLOAT32_3);
    m_VertexLayout->addAttribute("TexCoord", SR_FLOAT32_2);

    MeshData cube = createCube();

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    vboDesc.data = cube.vertices.data();
    vboDesc.layout = m_VertexLayout;
    vboDesc.count = cube.vertices.size();
    m_VertexBuffer = m_Context->createVertexBuffer(vboDesc);

    IndexBufferDesc iboDesc;
    iboDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    iboDesc.data = cube.indices.data();
    iboDesc.count = cube.indices.size();
    iboDesc.dataType = SR_UINT32;
    m_IndexBuffer = m_Context->createIndexBuffer(iboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = shader->getProgram().vertexShader;
    vaoDesc.vertexBuffer = m_VertexBuffer;
    vaoDesc.indexBuffer = m_IndexBuffer;
    m_VertexArray = m_Context->createVertexArray(vaoDesc);
}

void MeshComponent::onUpdate(Duration deltaTime){
    m_TransformBuffer->bindShaderResource(0);
    m_Context->draw(m_VertexArray);
}

void MeshComponent::onImGui(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "Mesh");
    ImGui::Text("Vertex Count: %d", m_VertexBuffer->getCount());
    ImGui::Text("Index Count: %d", m_IndexBuffer->getCount());

    imGuiEnd(windowData);
}
