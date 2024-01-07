#include "Projection.hpp"

Projection::Projection(ResourceView<Context> &context, uint32 width, uint32 height, float fov, float near, float far):
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


Projection::~Projection() {

}

void Projection::bind(uint32 slot) {
    m_Resource->bind(slot);
}

void Projection::onResize(uint32 width, uint32 height) {
    m_Width = static_cast<float>(width);
    m_Height = static_cast<float>(height);
    m_ProjectionData.m_Perspective = glm::perspective(glm::radians(m_Fov), m_Width / m_Height, m_Near, m_Far);
    m_ProjectionData.m_Orthographic = glm::ortho(0.0f, m_Width, 0.0f, m_Height, m_Near, m_Far);
    m_Resource->setData(&m_ProjectionData);
}

