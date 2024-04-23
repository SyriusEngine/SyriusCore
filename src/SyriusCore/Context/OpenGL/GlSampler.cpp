#include "GlSampler.hpp"

namespace Syrius{

    GlSampler::GlSampler(const SamplerDesc &desc):
    Sampler(desc),
    m_SamplerID(0){
        glCreateSamplers(1, &m_SamplerID);

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, getGlTextureWrap(m_WrapU));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, getGlTextureWrap(m_WrapV));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_R, getGlTextureWrap(m_WrapW));

        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, getGlTextureFilter(m_MinFilter));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, getGlTextureFilter(m_MagFilter));

        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MIN_LOD, m_MinLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MAX_LOD, m_MaxLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_LOD_BIAS, m_LODBias);

        glSamplerParameterfv(m_SamplerID, GL_TEXTURE_BORDER_COLOR, m_BorderColor);
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_COMPARE_FUNC, getGlComparisonFunc(m_ComparisonFunc));
    }

    GlSampler::~GlSampler() {
        glDeleteSamplers(1, &m_SamplerID);
    }

    void GlSampler::bindShaderResource(uint32 slot) {
        glBindSampler(slot, m_SamplerID);
    }

    void GlSampler::setFilter(SR_TEXTURE_FILTER minFilter, SR_TEXTURE_FILTER magFilter) {
        m_MinFilter = minFilter;
        m_MagFilter = magFilter;
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, getGlTextureFilter(m_MinFilter));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, getGlTextureFilter(m_MagFilter));
    }

    void GlSampler::setWrap(SR_TEXTURE_WRAP wrapU, SR_TEXTURE_WRAP wrapV, SR_TEXTURE_WRAP wrapW) {
        m_WrapU = wrapU;
        m_WrapV = wrapV;
        m_WrapW = wrapW;
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, getGlTextureWrap(m_WrapU));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, getGlTextureWrap(m_WrapV));
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_R, getGlTextureWrap(m_WrapW));
    }

    void GlSampler::setComparisonFunc(SR_COMPARISON_FUNC comparisonFunc) {
        m_ComparisonFunc = comparisonFunc;
        glSamplerParameteri(m_SamplerID, GL_TEXTURE_COMPARE_FUNC, getGlComparisonFunc(m_ComparisonFunc));
    }

    void GlSampler::setBorderColor(const float *borderColor) {
        m_BorderColor[0] = borderColor[0];
        m_BorderColor[1] = borderColor[1];
        m_BorderColor[2] = borderColor[2];
        m_BorderColor[3] = borderColor[3];
        glSamplerParameterfv(m_SamplerID, GL_TEXTURE_BORDER_COLOR, m_BorderColor);
    }

    void GlSampler::setLOD(float minLOD, float maxLOD, float LODBias) {
        m_MinLOD = minLOD;
        m_MaxLOD = maxLOD;
        m_LODBias = LODBias;
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MIN_LOD, m_MinLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_MAX_LOD, m_MaxLOD);
        glSamplerParameterf(m_SamplerID, GL_TEXTURE_LOD_BIAS, m_LODBias);
    }

    uint64 GlSampler::getIdentifier() {
        return m_SamplerID;
    }
}
