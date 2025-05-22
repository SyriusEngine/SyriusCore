#include "MeshComponent.hpp"

#include "../Utils.hpp"

MeshComponent::MeshComponent(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer* container):
IComponent(window, context, container){
    ConstantBufferDesc transformDesc;
    transformDesc.data = &m_Transform;
    transformDesc.size = sizeof(glm::mat4);
    transformDesc.shaderStage = SR_SHADER_VERTEX;
    transformDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    transformDesc.name = "TransformData";
    m_TransformBuffer = m_Context->createConstantBuffer(transformDesc);
}

void MeshComponent::onUpdate(Duration deltaTime){

}

void MeshComponent::onImGui(ImGuiWindowData &windowData){

}
