#pragma once

#include "Utils.hpp"

struct ProjectionData{
    glm::mat4 m_Perspective;
    glm::mat4 m_Orthographic;
};

class Projection{
public:
    Projection(ResourceView<Context>& context, uint32 width, uint32 height, float fov = 45.0f, float near = 0.1f, float far = 100.0f);

    ~Projection();

    void bind(uint32 slot);

    void onResize(uint32 width, uint32 height);

private:
    ResourceView<Context>& m_Context;

    ProjectionData m_ProjectionData;

    ResourceView<ConstantBuffer> m_Resource;



};
