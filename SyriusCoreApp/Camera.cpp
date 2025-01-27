#include "Camera.hpp"

struct CameraData{
    glm::mat4 view = glm::mat4(1.0f);
    glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
};

Camera::Camera(float sensitivity, float speed, const ResourceView<Context>& context):
m_Position(glm::vec3(0.0f, 0.0f, 3.0f)),
m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
m_Right(glm::vec3(1.0f, 0.0f, 0.0f)),
m_WorldUp({0.0f, 1.0f, 0.0f}),
m_Yaw(-90.0f),
m_Pitch(0.0f),
m_Speed(speed),
m_Sensitivity(sensitivity),
m_Context(context),
m_Enable(false){
    CameraData data;
    ConstantBufferDesc desc;
    desc.size = sizeof(CameraData);
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.data = &data;
    desc.name = "CameraData";
    desc.shaderStage = SR_SHADER_VERTEX;
    m_CameraData = m_Context->createConstantBuffer(desc);
    updateCameraData();
}

Camera::~Camera() = default;

void Camera::update(const Event &event, double deltaTime) {
    switch (event.type) {
        case SR_EVENT_RAW_MOUSE_MOVED: {
            auto xPos = static_cast<float>(event.mousePosX);
            auto yPos = static_cast<float>(event.mousePosY);
            if (m_Enable){
                mouseMoved(xPos, yPos);
            }
            break;
        }
        case SR_EVENT_KEYBOARD_KEY_PRESSED: {
            auto key = event.keyCode;
            if (key == SR_KEY_F){
                m_Enable = !m_Enable;
            }
            if (m_Enable){
                move(key, deltaTime);
            }
            break;
        }
        default:
            break;
    }
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

void Camera::move(i32 keyPressed, double deltaTime) {
    float velocity = static_cast<float>(deltaTime) * m_Speed;
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

void Camera::bind(u32 slot) {
    m_CameraData->bind(slot);
}

void Camera::setPosition(const glm::vec3 &position) {
    m_Position = position;
    updateCameraData();
}

bool Camera::isEnabled() const {
    return m_Enable;
}
