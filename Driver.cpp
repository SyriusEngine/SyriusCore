#include "Driver.hpp"

Camera::Camera(float sensitivity, float speed)
    : m_Position(glm::vec3(0.0f, 0.0f, 2.0f)),
      m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
      m_Yaw(-90.0f),
      m_Pitch(0.0f),
      m_Speed(speed),
      m_Sensitivity(sensitivity),
      m_LastX(0.0f),
      m_LastY(0.0f),
      m_FirstMouse(true) {}

Camera::~Camera() {

}

void Camera::update(float mouseX, float mouseY) {
    if (m_FirstMouse){
        m_LastX = mouseX;
        m_LastY = mouseY;
        m_FirstMouse = false;
    }
    float xOffset = mouseX - m_LastX;
    float yOffset = m_LastY - mouseY;
    m_LastX = mouseX;
    m_LastY = mouseY;

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

    glm::vec3 front;
    front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
    front.y = glm::sin(glm::radians(m_Pitch));
    front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

    m_Front = glm::normalize(front);
}

void Camera::moveForward() {
    m_Position += m_Front * m_Speed;
}

void Camera::moveBackwards() {
    m_Position -= m_Front * m_Speed;
}

void Camera::moveLeft() {
    m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * m_Speed;
}

void Camera::moveRight() {
    m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * m_Speed;
}

void Camera::moveUp() {
    m_Position += m_Up * m_Speed;
}

void Camera::moveDown() {
    m_Position -= m_Up * m_Speed;
}

glm::mat4 Camera::getViewMatrix() {
    auto mat = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    return mat;
}
