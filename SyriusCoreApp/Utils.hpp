#pragma once

#include "Include.hpp"

struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
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

Mesh createRectangle();

Mesh createSphere(uint32 rings, uint32 sectors);

Mesh createCube();

void printEventInfo(const Event& event);

void printContextInfo(const ResourceView<Context>& context);