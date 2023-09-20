#pragma once

#include "Include.hpp"

struct Vertex{
    float m_Position[3];
    float m_TexCoords[2];
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

void printEventInfo(const Event& event);

void printContextInfo(const ResourceView<Context>& context);