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
        const char* code = nullptr;
        if (desc.m_LoadType == SR_LOAD_FROM_FILE){
            code = readFile(desc.m_Code).c_str();
        }
        else{
            code = desc.m_Code.c_str();
        }
        int32 success;
        char infoLog[512];

        m_ModuleID = glCreateShader(getGlShaderType(desc.m_Type));
        glShaderSource(m_ModuleID, 1, &code, nullptr);
        glCompileShader(m_ModuleID);
        glGetShaderiv(m_ModuleID, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(m_ModuleID, 512, nullptr, infoLog);
            std::string errormsg = "[GlShaderModule]: Failed to compile shader, type = " + std::to_string(desc.m_Type) + ", error = " + infoLog;
            SR_CORE_WARNING(errormsg);
        }
    }

    void GlShaderModule::loadSPIRV(const ShaderModuleDesc& desc) {
        const char* code = nullptr;
        if (desc.m_LoadType == SR_LOAD_FROM_FILE){
            code = readFileBinary(desc.m_Code).c_str();
        }
        else{
            code = desc.m_Code.c_str();
        }
        int32 success;
        char infoLog[512];
        m_ModuleID = glCreateShader(getGlShaderType(desc.m_Type));
        glShaderBinary(1, &m_ModuleID, GL_SHADER_BINARY_FORMAT_SPIR_V, code, desc.m_CodeLength);
        glSpecializeShader(m_ModuleID, desc.m_EntryPoint.c_str(), 0, nullptr, nullptr);

        glGetShaderiv(m_ModuleID, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(m_ModuleID, 512, nullptr, infoLog);
            std::string errormsg = "[GlShaderModule]: Failed to compile shader, type = " + std::to_string(desc.m_Type) + ", error = " + infoLog;
            SR_CORE_WARNING(errormsg);
        }
    }


}
