#include "GlShaderModule.hpp"

namespace Syrius{

    GlShaderModule::GlShaderModule(const ShaderModuleDesc &desc)
    : ShaderModule(desc),
    m_ModuleID(0){
        SR_CORE_PRECONDITION(desc.m_CodeType != SR_SHADER_CODE_HLSL, "HLSL is not supported in OpenGL");

        if (desc.m_CodeType == SR_SHADER_CODE_SPIRV){
            loadSPIRV(desc);
        }
        else{
            loadGlsl(desc);
        }

    }

    GlShaderModule::~GlShaderModule() {
        glDeleteShader(m_ModuleID);
    }

    uint32 GlShaderModule::getShaderModuleID() const {
        return m_ModuleID;
    }

    void GlShaderModule::loadGlsl(const ShaderModuleDesc& desc) {
        std::string code;
        if (desc.m_LoadType == SR_LOAD_FROM_FILE){
            code = readFile(desc.m_Code);
        }
        else{
            code = desc.m_Code;
        }
        int32 success;
        char infoLog[512];

        m_ModuleID = glCreateShader(getGlShaderType(desc.m_Type));
        const char* codePtr = code.c_str();
        glShaderSource(m_ModuleID, 1, &codePtr, nullptr);
        glCompileShader(m_ModuleID);
        glGetShaderiv(m_ModuleID, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(m_ModuleID, 512, nullptr, infoLog);
            std::string errormsg = "Failed to compile shader, type = " + std::to_string(desc.m_Type) + ", error = " + infoLog;
            SR_CORE_WARNING(errormsg);
        }
    }

    void GlShaderModule::loadSPIRV(const ShaderModuleDesc& desc) {
        std::string code;
        if (desc.m_LoadType == SR_LOAD_FROM_FILE){
            code = readFileBinary(desc.m_Code);
        }
        else{
            code = desc.m_Code;
        }
        int32 success;
        char infoLog[512];
        m_ModuleID = glCreateShader(getGlShaderType(desc.m_Type));
        const char* codePtr = code.c_str();
        glShaderBinary(1, &m_ModuleID, GL_SHADER_BINARY_FORMAT_SPIR_V, codePtr, code.size());
        glSpecializeShader(m_ModuleID, desc.m_EntryPoint.c_str(), 0, nullptr, nullptr);

        glGetShaderiv(m_ModuleID, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(m_ModuleID, 512, nullptr, infoLog);
            std::string errormsg = "Failed to compile shader, type = " + std::to_string(desc.m_Type) + ", error = " + infoLog;
            SR_CORE_WARNING(errormsg);
        }
    }


}
