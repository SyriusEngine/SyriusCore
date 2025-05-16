#pragma once

#include "SandBoxInclude.hpp"

void printContextInfo(const ResourceView<Context>& context);

void printEventInfo(const Event& event);

struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texCoord;
};

struct Mesh{
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
};

struct ShaderProgram{
    ResourceView<Shader> shaderProgram;
    ResourceView<ShaderModule> vertexShader;
    ResourceView<ShaderModule> fragmentShader;
};

ShaderProgram loadShader(const ResourceView<Context>& context, const std::string& vertexShader, const std::string& fragmentShader);

std::string drawModeToString(SR_DRAW_MODE mode);