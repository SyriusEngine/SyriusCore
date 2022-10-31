#include "GlSampler.hpp"

namespace Syrius{

    GlSampler::GlSampler(const SamplerDesc &desc)
    : Sampler(desc),
      m_SamplerID(0){
        glCreateSamplers(1, &m_SamplerID);

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, getGlTextureWrap(desc.m_WrapU));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, getGlTextureWrap(desc.m_WrapV));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_R, getGlTextureWrap(desc.m_WrapW));

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, getGlTextureFilter(desc.m_MinFilter));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, getGlTextureFilter(desc.m_MagFilter));

        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MIN_LOD, desc.m_MinLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MAX_LOD, desc.m_MaxLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_LOD_BIAS, desc.m_LODBias);

        glSamplerParameterfv(m_SamplerID, GL_TEXTURE_BORDER_COLOR, desc.m_BorderColor);
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_COMPARE_FUNC, getGlComparisonFunc(desc.m_ComparisonFunc));
    }

    GlSampler::~GlSampler() {
        glDeleteSamplers(1, &m_SamplerID);
    }

    void GlSampler::bind(uint32 slot) {
        glBindSampler(slot, m_SamplerID);
    }

    uint64 GlSampler::getIdentifier() {
        return m_SamplerID;
    }
}
