#include "GlShader.hpp"

namespace Syrius{

    GlShader::GlShader(const ShaderDesc& desc):
    Shader(desc),
    m_ShaderID(0){
        m_ShaderID = glCreateProgram();

        glAttachShader(m_ShaderID, desc.vertexShader->getIdentifier());
        glAttachShader(m_ShaderID, desc.fragmentShader->getIdentifier());

        i32 success;
        char infoLog[512];
        glLinkProgram(m_ShaderID);
        glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_ShaderID, 512, nullptr, infoLog);
            SR_LOG_THROW("GlShader", "Failed to link shader program, error: %s", infoLog);
        }
    }

    GlShader::~GlShader() {
        glDeleteProgram(m_ShaderID);
    }

    void GlShader::bind() {
        glUseProgram(m_ShaderID);
    }

    u64 GlShader::getIdentifier() const {
        return m_ShaderID;
    }
}
