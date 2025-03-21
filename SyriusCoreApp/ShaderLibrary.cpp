#include "ShaderLibrary.hpp"

ShaderLibrary::ShaderLibrary(const std::string &libraryPath, const ResourceView<Context> &context):
m_Context(context),
m_LibraryPath(libraryPath) {
    // remove \r from path
    m_LibraryPath.erase(std::remove(m_LibraryPath.begin(), m_LibraryPath.end(), '\r'), m_LibraryPath.end());

}

ShaderLibrary::~ShaderLibrary() {

}

ShaderProgram ShaderLibrary::loadShader(const std::string& vertexShader, const std::string& fragmentShader) {
    if (vertexShader.empty()){
        throw std::runtime_error("ShaderLibrary::loadShader: Vertex shader name is empty");
    }
    if (fragmentShader.empty()){
        return loadShader(vertexShader, vertexShader);
    }

    auto apiPath = m_LibraryPath;
    std::string extension;
    SR_SHADER_LANGUAGE_TYPE codeType;
    switch (m_Context->getType()){
        case SR_API_OPENGL: {
            apiPath += "/GLSL/";
            extension = ".glsl";
            codeType = SR_SHADER_LANGUAGE_GLSL;
            break;
        }
        case SR_API_D3D11:{
            apiPath += "/HLSL/";
            extension = ".hlsl";
            codeType = SR_SHADER_LANGUAGE_HLSL;
            break;
        }
        default:
            throw std::runtime_error("ShaderLibrary::loadShader: Unsupported API");
    }

    ShaderProgram program;

    ShaderModuleFileDesc vsDesc;
    vsDesc.shaderType = SR_SHADER_VERTEX;
    vsDesc.language = codeType;
    vsDesc.filePath = apiPath + vertexShader + "_vs" + extension;
    vsDesc.entryPoint = "main";
    program.vertexShader = m_Context->createShaderModule(vsDesc);

    ShaderModuleFileDesc fsDesc;
    fsDesc.shaderType = SR_SHADER_FRAGMENT;
    fsDesc.language = codeType;
    fsDesc.filePath = apiPath + fragmentShader + "_fs" + extension;
    fsDesc.entryPoint = "main";
    program.fragmentShader = m_Context->createShaderModule(fsDesc);

    ShaderDesc sDesc;
    sDesc.vertexShader = program.vertexShader;
    sDesc.fragmentShader = program.fragmentShader;
    program.shaderProgram = m_Context->createShader(sDesc);

    return program;

}
