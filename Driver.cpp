#include "Driver.hpp"
#include <iostream>

void eventDisplayer(const Event& event){
    switch (event.type){
        case SR_EVENT_WINDOW_CLOSED:
            std::cout << "Window closed event" << std::endl;
            break;
        case SR_EVENT_WINDOW_RESIZED:
            std::cout << "Window resized event: " << event.windowWidth << "x" << event.windowHeight << std::endl;
            break;
        case SR_EVENT_WINDOW_MOVED:
            std::cout << "Window moved event: " << event.windowPosX << "x" << event.windowPosY << std::endl;
            break;
        case SR_EVENT_WINDOW_GAINED_FOCUS:
            std::cout << "Window focus gained event" << std::endl;
            break;
        case SR_EVENT_WINDOW_LOST_FOCUS:
            std::cout << "Window focus lost event" << std::endl;
            break;
        case SR_EVENT_WINDOW_FILE_DROPPED:
            std::cout << "Window file dropped event: " << event.droppedFilePath << std::endl;
            break;
        case SR_EVENT_MOUSE_ENTERED:
            std::cout << "Window mouse entered event" << std::endl;
            break;
        case SR_EVENT_MOUSE_LEFT:
            std::cout << "Window mouse left event" << std::endl;
            break;
        case SR_EVENT_MOUSE_MOVED:
            std::cout << "Window mouse moved event: " << event.mousePosX << "x" << event.mousePosY << std::endl;
            break;
        case SR_EVENT_MOUSE_SCROLLED:
            std::cout << "Window mouse scrolled event: " << event.mouseScrollX << "x" << event.mouseScrollY << std::endl;
            break;
        case SR_EVENT_MOUSE_BUTTON_PRESSED:
            std::cout << "Window mouse button pressed event: " << event.mouseButton << std::endl;
            break;
        case SR_EVENT_MOUSE_BUTTON_RELEASED:
            std::cout << "Window mouse button released event: " << event.mouseButton << std::endl;
            break;
        case SR_EVENT_KEYBOARD_KEY_PRESSED:
            std::cout << "Window keyboard key pressed event: " << event.keyCode << std::endl;
            break;
        case SR_EVENT_KEYBOARD_KEY_RELEASED:
            std::cout << "Window keyboard key released event: " << event.keyCode << std::endl;
            break;
        case SR_EVENT_KEYBOARD_KEY_TYPED:
            std::cout << "Window keyboard key typed event: " << event.keyCode << std::endl;
            break;
        default:
            std::cout << "Unknown event" << std::endl;
            break;
    }
}

Mesh createRectangle(){
    Mesh mesh;
    mesh.vertices = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f},  {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f},   {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f},  {0.0f, 1.0f}}
    };
    mesh.indices = {
            0, 3, 2,
            0, 2, 1
    };
    return mesh;
}

#if defined(SR_COMPILER_MSVC)
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#endif

Mesh createSphere(uint32 rings, uint32 sectors){
    Mesh mesh;
    float const R = 1.0f/(float)(rings-1);
    float const S = 1.0f/(float)(sectors-1);
    uint32 r, s;

    for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            mesh.vertices.push_back({{x, y, z},  {s*S, r*R}});
        }

    for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
            mesh.indices.push_back(r * sectors + s);
            mesh.indices.push_back(r * sectors + (s+1));
            mesh.indices.push_back((r+1) * sectors + (s+1));

            mesh.indices.push_back(r * sectors + s);
            mesh.indices.push_back((r+1) * sectors + (s+1));
            mesh.indices.push_back((r+1) * sectors + s);
        }
    return mesh;
}

ShaderProgram loadShader(const std::string& vertexPath, const std::string& fragmentPath, SR_SHADER_CODE_TYPE type, Context* context){
    ShaderProgram prg;

    ShaderModuleDesc vsDesc;
    vsDesc.shaderType = SR_SHADER_VERTEX;
    vsDesc.codeType = type;
    vsDesc.code = vertexPath;
    vsDesc.loadType = SR_LOAD_FROM_FILE;
    vsDesc.entryPoint = "main";
    vsDesc.codeLength = 0;
    prg.vertexShader = context->createShaderModule(vsDesc);

    ShaderModuleDesc fsDesc;
    fsDesc.shaderType = SR_SHADER_FRAGMENT;
    fsDesc.codeType = type;
    fsDesc.code = fragmentPath;
    fsDesc.loadType = SR_LOAD_FROM_FILE;
    fsDesc.entryPoint = "main";
    fsDesc.codeLength = 0;
    prg.fragmentShader = context->createShaderModule(fsDesc);

    ShaderDesc sDesc;
    sDesc.vertexShader = std::move(prg.vertexShader);
    sDesc.fragmentShader = std::move(prg.fragmentShader);
    prg.shaderProgram = context->createShader(sDesc);

    return prg;
}

ResourceView<VertexArray> loadMesh(Mesh& mesh, ShaderProgram& prg, Context* context){
    auto layout = context->createVertexDescription();
    layout->addAttribute("Position", SR_FLOAT32_3);
    layout->addAttribute("TexCoord", SR_FLOAT32_2);

    VertexBufferDesc vboDesc;
    vboDesc.type = SR_BUFFER_DEFAULT;
    vboDesc.data = &mesh.vertices[0];
    vboDesc.layout = layout;
    vboDesc.count = mesh.vertices.size();
    auto vbo = context->createVertexBuffer(vboDesc);

    IndexBufferDesc iboDesc;
    iboDesc.data = &mesh.indices[0];
    iboDesc.count = mesh.indices.size();
    iboDesc.type = SR_BUFFER_DEFAULT;
    iboDesc.dataType = SR_UINT32;
    auto ibo = context->createIndexBuffer(iboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = prg.vertexShader;
    vaoDesc.vertexBuffer = vbo;
    vaoDesc.indexBuffer = ibo;
    auto vao = context->createVertexArray(vaoDesc);

    return vao;
}

void printInfo(Context* context){
    std::string deviceInfo = "\n";
    deviceInfo += "Device name                          : " + context->getDeviceName() + "\n";
    deviceInfo += "API Version                          : " + context->getAPIVersion() + "\n";
    deviceInfo += "Shading language version             : " + context->getShadingLanguageVersion() + "\n";
    deviceInfo += "Max Framebuffer width                : " + std::to_string(context->getMaxFramebufferWidth()) + "\n";
    deviceInfo += "Max Framebuffer width                : " + std::to_string(context->getMaxFramebufferHeight()) + "\n";
    deviceInfo += "Max framebuffer texture attachments  : " + std::to_string(context->getMaxFramebufferTextureAttachments()) + "\n";
    deviceInfo += "Max Texture Slots                    : " + std::to_string(context->getMaxTextureSlots()) + " slots\n";
    deviceInfo += "Max Texture2D Size                   : " + std::to_string(context->getMaxTexture2DSize()) + " bytes\n";
    deviceInfo += "Max constant buffer size             : " + std::to_string(context->getMaxConstantBufferSize()) + " bytes\n";
    deviceInfo += "Depth buffer depth                   : " + std::to_string(context->getMaxDepthBufferBits()) + " bits\n";

    std::cout << deviceInfo << std::endl;
}

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
    mouseX *= m_Sensitivity;
    mouseY *= m_Sensitivity;
    m_Yaw += mouseX;
    m_Pitch += mouseY;
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
