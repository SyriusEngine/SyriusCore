#include "GlShader.hpp"

namespace Syrius{

    GlShader::GlShader()
    : Shader(),
    m_ShaderID(0){
        m_ShaderID = glCreateProgram();
    }

    GlShader::~GlShader() {
        glDeleteProgram(m_ShaderID);
    }

    void GlShader::addShaderModule(ShaderModule *module) {
        auto glModule = dynamic_cast<GlShaderModule*>(module);
        if (glModule != nullptr) {
            glAttachShader(m_ShaderID, glModule->getShaderModuleID());
        }
        else{
            SR_CORE_WARNING("Failed to add shader module to shader");
        }
    }

    void GlShader::link() {
        int32 success;
        char infoLog[512];
        glLinkProgram(m_ShaderID);
        glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_ShaderID, 512, nullptr, infoLog);
            std::string error = "[GlShader]: Failed to link shader program, error: " + std::string(infoLog);
            SR_CORE_WARNING(error);
        }
    }

    void GlShader::bind() {
        glUseProgram(m_ShaderID);
    }

    uint64 GlShader::getIdentifier() const {
        return m_ShaderID;
    }
}
