#pragma once

#include "Utils.hpp"

class Camera {
public:

    Camera(float sensitivity, float speed, const ResourceView<Context>& context);

    ~Camera();

    void update(const Event& event, float deltaTime);

private:
    float m_Sensitivity;
    float m_Speed;
    float m_Yaw;
    float m_Pitch;
    float m_LastX;
    float m_LastY;

    bool m_FirstMouse;

    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;

    glm::mat4 m_ViewMat;

    const ResourceView<Context>& m_Context;
    ResourceView<ConstantBuffer> m_CameraData;
};