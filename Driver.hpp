#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "include/SyriusCore/SyriusCore.hpp"

class Camera{
public:
    Camera(float sensitivity = 0.1f, float speed = 0.1f);

    ~Camera();

    void update(float mouseX, float mouseY);

    void moveForward();

    void moveBackwards();

    void moveLeft();

    void moveRight();

    void moveUp();

    void moveDown();

    glm::mat4 getViewMatrix();

private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;

    float m_Yaw;
    float m_Pitch;
    float m_Speed;
    float m_Sensitivity;
    float m_LastX;
    float m_LastY;

    bool m_FirstMouse;

};


class Driver {

};



