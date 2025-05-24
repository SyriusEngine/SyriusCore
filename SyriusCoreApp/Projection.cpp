#include "Projection.hpp"

ProjectionComponent::ProjectionComponent(ResourceView<Context> &context, u32 width, u32 height, float fov, float near, float far):
m_Context(context),
m_Width(static_cast<float>(width)),
m_Height(static_cast<float>(height)),
m_Fov(fov),
m_Near(near),
m_Far(far){
    m_ProjectionData.m_Perspective = glm::perspective(glm::radians(m_Fov), m_Width / m_Height, m_Near, m_Far);
    m_ProjectionData.m_Orthographic = glm::ortho(0.0f, m_Width, 0.0f, m_Height, m_Near, m_Far);

    ConstantBufferDesc desc;
    desc.shaderStage = SR_SHADER_VERTEX;
    desc.size = sizeof(ProjectionData);
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.name = "ProjectionData";
    desc.data = &m_ProjectionData;
    m_Resource = m_Context->createConstantBuffer(desc);
}


ProjectionComponent::~ProjectionComponent() {

}

void ProjectionComponent::bind(u32 slot) {
    m_Resource->bindShaderResource(slot);
}

void ProjectionComponent::onEvent(const Event& event) {
    if (event.type == SR_EVENT_WINDOW_RESIZED) {
        m_Width = static_cast<float>(event.windowWidth);
        m_Height = static_cast<float>(event.windowHeight);
        m_ProjectionData.m_Perspective = glm::perspective(glm::radians(m_Fov), m_Width / m_Height, m_Near, m_Far);
        m_ProjectionData.m_Orthographic = glm::ortho(0.0f, m_Width, 0.0f, m_Height, m_Near, m_Far);
        m_Resource->setData(&m_ProjectionData, sizeof(ProjectionData));
    }
}

