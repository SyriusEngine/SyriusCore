#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../include/SyriusCore/SyriusCore.hpp"

using namespace Syrius;

struct Vertex{
    float m_Position[3];
    float m_TexCoords[2];
};

struct Mesh{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
};

Mesh createRectangle();

Mesh createSphere(uint32 rings, uint32 sectors);

struct ShaderProgram{
    ResourceView<Shader> shaderProgram;
    ResourceView<ShaderModule> vertexShader;
    ResourceView<ShaderModule> fragmentShader;
};

ShaderProgram loadShader(const std::string& vertexPath, const std::string& fragmentPath, SR_SHADER_CODE_TYPE type, ResourceView<Context>& context);

ResourceView<VertexArray> loadMesh(Mesh& mesh, ShaderProgram& prg, ResourceView<Context>& context);

