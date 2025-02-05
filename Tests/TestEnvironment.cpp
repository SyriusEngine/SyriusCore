#include "TestEnvironment.hpp"

UP<SyriusWindow> TestEnvironment::m_Window;
ResourceView<Context> TestEnvironment::m_Context;
SR_SUPPORTED_API TestEnvironment::m_API = SR_API_NONE;
bool TestEnvironment::m_ExportImages = false;

void TestEnvironment::setup(const EnvironmentDesc &desc) {
    WindowDesc windowDesc;
    windowDesc.width = desc.width;
    windowDesc.height = desc.height;
    windowDesc.title = desc.title;
    m_Window = createWindow(windowDesc);

    ContextDesc contextDesc;
    contextDesc.api = desc.api;
    m_Context = m_Window->createContext(contextDesc);

    m_API = desc.api;
    m_ExportImages = desc.exportImages;
}

void TestEnvironment::teardown() {
    m_Window->destroyContext();
    m_Window.reset();
    m_Window = nullptr;
}

ShaderModuleFileDesc TestEnvironment::createShaderModuleDescFromFileVS(const std::string &file) {
    ShaderModuleFileDesc desc;
    desc.entryPoint = "main";
    desc.shaderType = SR_SHADER_VERTEX;
    desc.filePath = fs::current_path() / "TestResources" / "Shaders";
    if (m_API == SR_API_OPENGL){
        desc.language = SR_SHADER_LANGUAGE_GLSL;
        const std::string full_file = file + std::string("_vs.glsl");
        desc.filePath /= "GLSL";
        desc.filePath /= full_file;
    }
    else if (m_API == SR_API_D3D11){
        desc.language = SR_SHADER_LANGUAGE_HLSL;
        const std::string full_file = file + std::string("_vs.hlsl");
        desc.filePath /= "HLSL";
        desc.filePath /= full_file;
    }
    else {
        throw std::runtime_error("Unsupported API");
    }
    return desc;
}

ShaderModuleFileDesc TestEnvironment::createShaderModuleDescFromFileFS(const std::string &file) {
    ShaderModuleFileDesc desc;
    desc.entryPoint = "main";
    desc.shaderType = SR_SHADER_FRAGMENT;
    desc.filePath = fs::current_path() / "TestResources" / "Shaders";
    if (m_API == SR_API_OPENGL){
        desc.language = SR_SHADER_LANGUAGE_GLSL;
        const std::string full_file = file + std::string("_fs.glsl");
        desc.filePath /= "GLSL";
        desc.filePath /= full_file;
    }
    else if (m_API == SR_API_D3D11){
        desc.language = SR_SHADER_LANGUAGE_HLSL;
        const std::string full_file = file + std::string("_fs.hlsl");
        desc.filePath /= "HLSL";
        desc.filePath /= full_file;
    }
    else {
        throw std::runtime_error("Unsupported API");
    }
    return desc;
}


