#pragma once

#include "Include.hpp"

struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texCoord;
};

struct Mesh{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
};

struct ShaderProgram{
    ResourceView<Shader> shaderProgram;
    ResourceView<ShaderModule> vertexShader;
    ResourceView<ShaderModule> fragmentShader;
};

const Vertex s_Triangle[] = {
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
    glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.5f, 1.0f),
    glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f),
};

const Vertex s_Rectangle[] = {
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f),
    glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f),
    glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f),

    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f),
    glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f),
    glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f),
};

Mesh createTriangle();

Mesh createRectangle(float size = 0.5f);

Mesh createScreenRectangle();

Mesh createSphere(uint32 rings, uint32 sectors);

Mesh createCube();

Mesh createBigCube();

void printEventInfo(const Event& event);

void printContextInfo(const ResourceView<Context>& context);

std::string drawModeToString(SR_DRAW_MODE mode);

std::string filterToString(SR_TEXTURE_FILTER mode);

std::string wrapModeToString(SR_TEXTURE_WRAP mode);

std::vector<uint8> createCheckerBoard(const float* color, uint32 width, uint32 height);