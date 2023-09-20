#include "Camera.hpp"

Camera::Camera(float sensitivity, float speed, const ResourceView<Context>& context):
m_Sensitivity(sensitivity),
m_Speed(speed),
m_Yaw(90.0f),
m_Pitch(0.0f),
m_LastX(0.0f),
m_LastY(0.0f),
m_FirstMouse(true),
m_Position({0.0f, 0.0f, 0.0f}),
m_Up({0.0f, 1.0f, 0.0f}),
m_Front({0.0f, 0.0f, 1.0f}),
m_ViewMat(glm::mat4(1.0f)),
m_Context(context){
    ConstantBufferDesc desc;
    desc.size = sizeof(glm::mat4);
    desc.type = SR_BUFFER_DYNAMIC;
    desc.data = &m_ViewMat;
    desc.slot = 0;
    desc.name = "CameraData";
    desc.shaderStage = SR_SHADER_VERTEX;
    m_CameraData = m_Context->createConstantBuffer(desc);

    m_CameraData->bind();

}

Camera::~Camera() {

}

void Camera::update(const Event &event, float deltaTime) {
    switch (event.type) {
        case SR_EVENT_RAW_MOUSE_MOVED: {
            if (m_FirstMouse){
                m_LastX = event.mousePosX;
                m_LastY = event.mousePosY;
                m_FirstMouse = false;
            }
            float xOffset = event.mousePosX - m_LastX;
            float yOffset = m_LastY - event.mousePosY;
            m_LastX = event.mousePosX;
            m_LastY = event.mousePosY;

            xOffset *= m_Sensitivity;
            yOffset *= m_Sensitivity;
            m_Yaw += xOffset;
            m_Pitch += yOffset;
            // when dragged to far, make sure the screen doesn't get flipped
            if (m_Pitch > 89.0f) {
                m_Pitch = 89.0f;
            }
            if (m_Pitch < -89.0f) {
                m_Pitch = -89.0f;
            }
            break;
        }
        case SR_EVENT_KEYBOARD_KEY_PRESSED: {
            switch (event.keyCode) {
                case SR_KEY_W: {
                    m_Position += m_Speed * m_Front * deltaTime;
                    break;
                }
                case SR_KEY_S: {
                    m_Position -= m_Speed * m_Front * deltaTime;
                    break;
                }
                case SR_KEY_A: {
                    m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * m_Speed * deltaTime;
                    break;
                }
                case SR_KEY_D: {
                    m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * m_Speed * deltaTime;
                    break;
                }
                case SR_KEY_C: {
                    m_Position -= m_Speed * m_Up * deltaTime;
                    break;
                }
                case SR_KEY_SPACE: {
                    m_Position += m_Speed * m_Up * deltaTime;
                    break;
                }
                default:
                    break;
            }

        }
        default:
            break;
    }
    glm::vec3 front;
    front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
    front.y = glm::sin(glm::radians(m_Pitch));
    front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

    m_Front = glm::normalize(front);
    m_ViewMat = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    m_CameraData->setData(&m_ViewMat);
}

void Camera::bind() {
    m_CameraData->bind();

}
