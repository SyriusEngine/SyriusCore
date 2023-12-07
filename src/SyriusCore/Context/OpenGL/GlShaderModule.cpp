#include "GlShaderModule.hpp"

namespace Syrius{

    GlShaderModule::GlShaderModule(const ShaderModuleDesc &desc)
    : ShaderModule(desc),
    m_ModuleID(0){
        SR_CORE_PRECONDITION(desc.codeType != SR_SHADER_CODE_HLSL, "HLSL is not supported in OpenGL");

        if (desc.codeType == SR_SHADER_CODE_SPIRV){
            loadSPIRV(desc);
        }
        else{
            loadGlsl(desc);
        }

    }

    GlShaderModule::~GlShaderModule() {
        glDeleteShader(m_ModuleID);
    }

    uint64 GlShaderModule::getIdentifier() const {
        return m_ModuleID;
    }

    void GlShaderModule::loadGlsl(const ShaderModuleDesc& desc) {
        std::string code;
        if (desc.loadType == SR_LOAD_FROM_FILE){
            code = readFile(desc.code);
        }
        else{
            code = desc.code;
        }
        int32 success;
        char infoLog[512];

        m_ModuleID = glCreateShader(getGlShaderType(desc.shaderType));
        const char* codePtr = code.c_str();
        glShaderSource(m_ModuleID, 1, &codePtr, nullptr);
        glCompileShader(m_ModuleID);
        glGetShaderiv(m_ModuleID, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(m_ModuleID, 512, nullptr, infoLog);
            SR_CORE_WARNING("Failed to compile shader, usage = %S, error = %s", getShaderTypeString(desc.shaderType).c_str(), infoLog);
        }
    }

    void GlShaderModule::loadSPIRV(const ShaderModuleDesc& desc) {
        std::string code;
        if (desc.loadType == SR_LOAD_FROM_FILE){
            code = readFileBinary(desc.code);
        }
        else{
            code = desc.code;
        }
        int32 success;
        char infoLog[512];
        m_ModuleID = glCreateShader(getGlShaderType(desc.shaderType));
        const char* codePtr = code.c_str();
        glShaderBinary(1, &m_ModuleID, GL_SHADER_BINARY_FORMAT_SPIR_V, codePtr, code.size());
        glSpecializeShader(m_ModuleID, desc.entryPoint.c_str(), 0, nullptr, nullptr);

        glGetShaderiv(m_ModuleID, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(m_ModuleID, 512, nullptr, infoLog);
            SR_CORE_WARNING("Failed to compile shader, usage = %S, error = %s", getShaderTypeString(desc.shaderType).c_str(), infoLog);
        }
    }


}
