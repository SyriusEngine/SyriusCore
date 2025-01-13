#pragma once

#include "Utils.hpp"

struct ProjectionData{
    glm::mat4 m_Perspective;
    glm::mat4 m_Orthographic;
};

class Projection{
public:
    Projection(ResourceView<Context>& context, u32 width, u32 height, float fov = 45.0f, float near = 0.1f, float far = 100.0f);

    ~Projection();

    void bind(u32 slot);

    void onResize(u32 width, u32 height);

private:
    ResourceView<Context>& m_Context;

    ProjectionData m_ProjectionData;

    ResourceView<ConstantBuffer> m_Resource;

    float m_Fov;
    float m_Near;
    float m_Far;
    float m_Width;
    float m_Height;



};
