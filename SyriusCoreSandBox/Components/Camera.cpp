#include "Camera.hpp"

struct CameraData{
    glm::mat4 view = glm::mat4(1.0f);
    glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
};


Camera::Camera(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer* container):
IComponent(window, context, container){

    CameraData zero;
    ConstantBufferDesc desc;
    desc.size = sizeof(CameraData);
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.data = &zero;
    desc.name = "CameraData";
    desc.shaderStage = SR_SHADER_VERTEX;
    m_CameraData = m_Context->createConstantBuffer(desc);

    updateCameraData();
}

void Camera::onUpdate(const Duration deltaTime){
    m_DeltaTime = deltaTime;

    m_CameraData->bindShaderResource(1);
}

void Camera::onEvent(const Event &event){
    switch (event.type) {
        case SR_EVENT_MOUSE_MOVED: {
            const float xPosition = static_cast<float>(event.mousePosX);
            const float yPosition = static_cast<float>(event.mousePosY);
            if (m_Enable) {
                mouseMoved(xPosition, yPosition);
            }
            break;
        }
        case SR_EVENT_KEYBOARD_KEY_PRESSED: {
            const SR_KEYBOARD_KEY key = static_cast<SR_KEYBOARD_KEY>(event.keyCode);
            if (key == SR_KEY_E) {
                m_Enable = !m_Enable;
            }
            if (m_Enable) {
                move(key);
            }
            break;
        }
        default: break;
    }
}

void Camera::onImGui(ImGuiWindowData &windowData){
    imGuiBegin(windowData, "Camera");
    if (m_Enable){
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Camera enabled");
    }
    else{
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Camera disabled");
    }
    ImGui::DragFloat("Speed", &m_Speed, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Sensitivity", &m_Sensitivity, 0.01f, 0.0f, 10.0f);
    imGuiEnd(windowData);
}

void Camera::updateCameraData() {
    glm::vec3 front;
    front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
    front.y = glm::sin(glm::radians(m_Pitch));
    front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));

    CameraData data;
    data.view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    data.position = glm::vec4(m_Position, 1.0f);
    m_CameraData->setData(&data, sizeof(CameraData));
}

void Camera::mouseMoved(float mousePosX, float mousePosY) {
    mousePosX *= m_Sensitivity;
    mousePosY *= m_Sensitivity;

    m_Yaw += mousePosX;
    m_Pitch -= mousePosY;

    if (m_Pitch > 89.0f) {
        m_Pitch = 89.0f;
    }
    if (m_Pitch < -89.0f) {
        m_Pitch = -89.0f;
    }

    updateCameraData();
}

void Camera::move(SR_KEYBOARD_KEY keyPressed) {
    const float velocity = static_cast<float>(m_DeltaTime.count()) * m_Speed;
    switch (keyPressed) {
        case SR_KEY_W:  m_Position += m_Front * velocity; break;
        case SR_KEY_S:  m_Position -= m_Front * velocity; break;
        case SR_KEY_A:  m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * velocity; break;
        case SR_KEY_D:  m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * velocity; break;
        case SR_KEY_SPACE:  m_Position += m_Up * velocity; break;
        case SR_KEY_LEFT_CONTROL:  m_Position -= m_Up * velocity; break;
        default: break;
    }
    updateCameraData();
}

