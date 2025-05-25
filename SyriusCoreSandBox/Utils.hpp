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

struct MeshData{
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
};

struct ShaderProgram{
    ResourceView<Shader> shaderProgram;
    ResourceView<ShaderModule> vertexShader;
    ResourceView<ShaderModule> fragmentShader;
};

struct TextureParameters{
    float lerpFactor = 0.0f;
    float scale = 1.0f;
    float padding[2] = {0.0f, 0.0f};
};

ShaderProgram loadShader(const ResourceView<Context>& context, const std::string& vertexShader, const std::string& fragmentShader);

ResourceView<VertexArray> loadMesh(const ResourceView<Context>& context, const MeshData& data, const ShaderProgram& program);

MeshData createTriangle(float size = 0.5f);

MeshData createRectangle(float size = 0.5f);

MeshData createScreenRectangle();

MeshData createSphere(u32 rings, u32 sectors);

MeshData createCube();

MeshData createBigCube();

std::string drawModeToString(SR_DRAW_MODE mode);

std::string filterToString(SR_TEXTURE_FILTER mode);

std::string wrapModeToString(SR_TEXTURE_WRAP mode);