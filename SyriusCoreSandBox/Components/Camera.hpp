#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class Camera: public IComponent {
public:
    Camera(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~Camera() override = default;

    void onUpdate(Duration deltaTime) override;

    void onEvent(const Event &event) override;

    void onImGui(ImGuiWindowData& windowData) override;

private:

    void updateCameraData();

    void mouseMoved(float mousePosX, float mousePosY);

    void move(SR_KEYBOARD_KEY keyPressed);

private:
    bool m_Enable = false;

    Duration m_DeltaTime = 0.0ms;

    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_Speed = 0.01f;
    float m_Sensitivity = 0.01f;

    ResourceView<ConstantBuffer> m_CameraData;
};

class CameraFactory: public IComponentFactory {
public:
    CameraFactory() = default;
    ~CameraFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<Camera>(window, context, container);
    }

    std::string provides() const override {
        return "Camera";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};