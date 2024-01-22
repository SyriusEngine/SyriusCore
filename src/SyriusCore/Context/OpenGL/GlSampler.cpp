#include "GlSampler.hpp"

namespace Syrius{

    GlSampler::GlSampler(const SamplerDesc &desc):
    Sampler(desc),
    m_SamplerID(0){
        glCreateSamplers(1, &m_SamplerID);

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, getGlTextureWrap(desc.wrapU));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, getGlTextureWrap(desc.wrapV));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_R, getGlTextureWrap(desc.wrapW));

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, getGlTextureFilter(desc.minFilter));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, getGlTextureFilter(desc.magFilter));

        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MIN_LOD, desc.minLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MAX_LOD, desc.maxLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_LOD_BIAS, desc.LODBias);

        glSamplerParameterfv(m_SamplerID, GL_TEXTURE_BORDER_COLOR, desc.borderColor);
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_COMPARE_FUNC, getGlComparisonFunc(desc.comparisonFunc));
    }

    GlSampler::~GlSampler() {
        glDeleteSamplers(1, &m_SamplerID);
    }

    void GlSampler::bindShaderResource(uint32 slot) {
        glBindSampler(slot, m_SamplerID);
    }

    uint64 GlSampler::getIdentifier() {
        return m_SamplerID;
    }
}
