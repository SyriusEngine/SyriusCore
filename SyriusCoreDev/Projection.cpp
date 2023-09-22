#include "Projection.hpp"

Projection::Projection(ResourceView<Context> &context, uint32 width, uint32 height, float fov, float near, float far):
m_Context(context){
    m_ProjectionData.m_Perspective = glm::perspective(glm::radians(fov), (float)width / (float)height, near, far);
    m_ProjectionData.m_Orthographic = glm::ortho(0.0f, (float)width, 0.0f, (float)height, near, far);

    ConstantBufferDesc desc;
    desc.shaderStage = SR_SHADER_VERTEX;
    desc.size = sizeof(ProjectionData);
    desc.type = SR_BUFFER_DYNAMIC;
    desc.name = "ProjectionData";
    desc.data = &m_ProjectionData;
    desc.slot = 0;
    m_Resource = m_Context->createConstantBuffer(desc);
}


Projection::~Projection() {

}

void Projection::bind() {
    m_Resource->bind();
}

