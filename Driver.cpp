#include "Driver.hpp"
#include <iostream>

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
    vsDesc.m_Type = SR_SHADER_VERTEX;
    vsDesc.m_CodeType = type;
    vsDesc.m_Code = vertexPath;
    vsDesc.m_LoadType = SR_LOAD_FROM_FILE;
    vsDesc.m_EntryPoint = "main";
    vsDesc.m_CodeLength = 0;
    prg.vertexShader = context->createShaderModule(vsDesc);

    ShaderModuleDesc fsDesc;
    fsDesc.m_Type = SR_SHADER_FRAGMENT;
    fsDesc.m_CodeType = type;
    fsDesc.m_Code = fragmentPath;
    fsDesc.m_LoadType = SR_LOAD_FROM_FILE;
    fsDesc.m_EntryPoint = "main";
    fsDesc.m_CodeLength = 0;
    prg.fragmentShader = context->createShaderModule(fsDesc);

    ShaderDesc sDesc;
    sDesc.m_VertexShader = prg.vertexShader;
    sDesc.m_FragmentShader = prg.fragmentShader;
    prg.shaderProgram = context->createShader(sDesc);

    return prg;
}

VertexArray* loadMesh(Mesh& mesh, ShaderProgram& prg, Context* context){
    auto layout = context->createVertexDescription();
    layout->addAttribute("Position", SR_FLOAT32_3);
    layout->addAttribute("TexCoord", SR_FLOAT32_2);

    VertexBufferDesc vboDesc;
    vboDesc.m_Type = SR_BUFFER_DEFAULT;
    vboDesc.m_Data = &mesh.vertices[0];
    vboDesc.m_Layout = layout;
    vboDesc.m_Count = mesh.vertices.size();
    auto vbo = context->createVertexBuffer(vboDesc);

    IndexBufferDesc iboDesc;
    iboDesc.m_Data = &mesh.indices[0];
    iboDesc.m_Count = mesh.indices.size();
    iboDesc.m_Type = SR_BUFFER_DEFAULT;
    iboDesc.m_DataType = SR_UINT32;
    auto ibo = context->createIndexBuffer(iboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.m_DrawMode = SR_DRAW_TRIANGLES;
    vaoDesc.m_VertexShader = prg.vertexShader;
    vaoDesc.m_VertexBuffer = vbo;
    vaoDesc.m_IndexBuffer = ibo;
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
