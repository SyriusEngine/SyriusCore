#pragma once

#include "Utils.hpp"

class Camera {
public:

    Camera(float sensitivity, float speed, const ResourceView<Context>& context);

    ~Camera();

    void update(const Event& event, double deltaTime);

    void bind(u32 slot);

    void setPosition(const glm::vec3& position);

    bool isEnabled() const;

private:

    void updateCameraData();

    void mouseMoved(float mousePosX, float mousePosY);

    void move(i32 keyPressed, double deltaTime);

private:
    bool m_Enable;

    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    float m_Yaw;
    float m_Pitch;
    float m_Speed;
    float m_Sensitivity;

    const ResourceView<Context>& m_Context;
    ResourceView<ConstantBuffer> m_CameraData;
};