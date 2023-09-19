#include "Driver.hpp"
#include "Camera.hpp"

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
            0, 3, 1,
            3, 2, 1
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

ShaderProgram loadShader(const std::string& vertexPath, const std::string& fragmentPath, SR_SHADER_CODE_TYPE type, ResourceView<Context>& context){
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

ResourceView<VertexArray> loadMesh(Mesh& mesh, ShaderProgram& prg, ResourceView<Context>& context){
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

