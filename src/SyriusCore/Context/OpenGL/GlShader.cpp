#include "GlShader.hpp"

namespace Syrius{

    GlShader::GlShader(const ShaderDesc& desc)
    : Shader(desc),
    m_ShaderID(0){
        m_ShaderID = glCreateProgram();

        glAttachShader(m_ShaderID, desc.m_VertexShader->getIdentifier());
        glAttachShader(m_ShaderID, desc.m_FragmentShader->getIdentifier());

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

    GlShader::~GlShader() {
        glDeleteProgram(m_ShaderID);
    }

    void GlShader::bind() {
        glUseProgram(m_ShaderID);
    }

    uint64 GlShader::getIdentifier() const {
        return m_ShaderID;
    }
}
