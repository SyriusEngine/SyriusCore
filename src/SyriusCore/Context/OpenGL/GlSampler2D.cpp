#include "GlSampler2D.hpp"

namespace Syrius{

    GlSampler2D::GlSampler2D(const Sampler2DDesc &desc)
    : Sampler2D(desc),
    m_SamplerID(0){
        glCreateSamplers(1, &m_SamplerID);

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, getGlTextureWrap(desc.m_WrapU));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, getGlTextureWrap(desc.m_WrapV));

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, getGlTextureFilter(desc.m_MinFilter));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, getGlTextureFilter(desc.m_MagFilter));
    }

    GlSampler2D::~GlSampler2D() {
        glDeleteSamplers(1, &m_SamplerID);
    }

    void GlSampler2D::bind(uint32 slot) {
        glBindSampler(slot, m_SamplerID);
    }

    uint64 GlSampler2D::getIdentifier() {
        return m_SamplerID;
    }
}
