#include "ProjectionComponent.hpp"

ProjectionComponent::ProjectionComponent(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer* container):
IComponent(window, context, container){

    ConstantBufferDesc desc;
    desc.shaderStage = SR_SHADER_VERTEX;
    desc.size = sizeof(ProjectionData);
    desc.count = 1;
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.name = "ProjectionData";
    desc.data = &m_ProjectionData;
    m_Buffer = m_Context->createConstantBuffer(desc);
    setProjection();
}

void ProjectionComponent::onUpdate(Duration deltaTime){
    m_Buffer->bindShaderResource(2);
}

void ProjectionComponent::onEvent(const Event &event){
    switch (event.type) {
        case SR_EVENT_WINDOW_RESIZED: {
            setProjection();
            break;
        }
        default:
            break;
    }
}

void ProjectionComponent::onImGui(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "Projection");

    if (ImGui::DragFloat("FOV", &m_Fov, 1.0f, 0.0f, 180.0f)){
        setProjection();
    }
    if (ImGui::DragFloat("ZNear", &m_Near, 0.1f, 0.0f, 100.0f)){
        setProjection();
    }
    if (ImGui::DragFloat("ZFar", &m_Far, 0.1f, 0.0f, 100.0f)){
        setProjection();
    }

    imGuiEnd(windowData);
}

void ProjectionComponent::setProjection(){
    const float width = static_cast<float>(m_Context->getWidth()); // Don't use the window width but the framebuffer width
    const float height = static_cast<float>(m_Context->getHeight()); // Don't use the window height but the framebuffer height
    m_ProjectionData.projection = glm::perspective(glm::radians(m_Fov), width / height, m_Near, m_Far);
    m_Buffer->setData(&m_ProjectionData, sizeof(ProjectionData));
}


