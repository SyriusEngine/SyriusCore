#include "GlShaderModule.hpp"

namespace Syrius{

    GlShaderModule::GlShaderModule(const ShaderModuleDesc &desc):
    ShaderModule(desc),
    m_ModuleID(0){
        SR_PRECONDITION(desc.language == SR_SHADER_LANGUAGE_SPRIV || desc.language == SR_SHADER_LANGUAGE_GLSL, "[GlShaderModule]: Invalid shader language type (%i), OpenGL only supports GLSL and SPIRV (if version is supported)", desc.language);

        if (desc.language == SR_SHADER_LANGUAGE_SPRIV){
            loadSPIRV(desc.code);
        }
        else{
            loadGlsl(desc.code);
        }
    }

    GlShaderModule::GlShaderModule(const ShaderModuleFileDesc &desc):
    ShaderModule(desc),
    m_ModuleID(0){
        SR_PRECONDITION(desc.language == SR_SHADER_LANGUAGE_SPRIV || desc.language == SR_SHADER_LANGUAGE_GLSL, "[GlShaderModule]: Invalid shader language type (%i), OpenGL only supports GLSL and SPIRV (if version is supported)", desc.language);

        SR_LOG_THROW_IF_FALSE(std::filesystem::exists(desc.filePath), "GlShaderModule", "Shader file (%s) does not exist", desc.filePath.c_str());

        if (desc.language == SR_SHADER_LANGUAGE_SPRIV){
            std::string code = readFileBinary(desc.filePath);
            loadSPIRV(code);
        }
        else{
            std::string code = readFile(desc.filePath);
            loadGlsl(code);
        }

    }

    GlShaderModule::~GlShaderModule() {
        glDeleteShader(m_ModuleID);
    }

    u64 GlShaderModule::getIdentifier() const {
        return m_ModuleID;
    }

    void GlShaderModule::loadGlsl(const std::string& code) {
        SR_PRECONDITION(!code.empty(), "[GlShaderModule]: Shader code is empty")

        i32 success;
        char infoLog[512];

        m_ModuleID = glCreateShader(getGlShaderType(m_ShaderType));
        const char* codePtr = code.c_str();
        glShaderSource(m_ModuleID, 1, &codePtr, nullptr);
        glCompileShader(m_ModuleID);
        glGetShaderiv(m_ModuleID, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(m_ModuleID, 512, nullptr, infoLog);
            SR_LOG_THROW("GlShaderModule", "Failed to compile shader, type = %s, error = %s", getShaderTypeString(m_ShaderType).c_str(), infoLog);
        }
    }

    void GlShaderModule::loadSPIRV(const std::string& code) {
        SR_PRECONDITION(!code.empty(), "[GlShaderModule]: Shader code is empty")

        i32 success;
        char infoLog[512];
        m_ModuleID = glCreateShader(getGlShaderType(m_ShaderType));
        const char* codePtr = code.c_str();
        glShaderBinary(1, &m_ModuleID, GL_SHADER_BINARY_FORMAT_SPIR_V, codePtr, code.size());
        glSpecializeShader(m_ModuleID, m_EntryPoint.c_str(), 0, nullptr, nullptr);

        glGetShaderiv(m_ModuleID, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(m_ModuleID, 512, nullptr, infoLog);
            SR_LOG_THROW("GlShaderModule", "Failed to compile shader, usage = %s, error = %s", getShaderTypeString(m_ShaderType).c_str(), infoLog);
        }
    }
}
