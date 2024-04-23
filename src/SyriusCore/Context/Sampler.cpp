#include "../../../include/SyriusCore/Context/Sampler.hpp"

namespace Syrius{

    Sampler::Sampler(const SamplerDesc &desc):
    m_MinFilter(desc.minFilter),
    m_MagFilter(desc.magFilter),
    m_WrapU(desc.wrapU),
    m_WrapV(desc.wrapV),
    m_WrapW(desc.wrapW),
    m_ComparisonFunc(desc.comparisonFunc),
    m_MinLOD(desc.minLOD),
    m_MaxLOD(desc.maxLOD),
    m_LODBias(desc.LODBias){

    }

    Sampler::~Sampler() = default;

    SR_TEXTURE_FILTER Sampler::getMinFilter() const {
        return m_MinFilter;
    }

    SR_TEXTURE_FILTER Sampler::getMagFilter() const {
        return m_MagFilter;
    }

    SR_TEXTURE_WRAP Sampler::getWrapU() const {
        return m_WrapU;
    }

    SR_TEXTURE_WRAP Sampler::getWrapV() const {
        return m_WrapV;
    }

    SR_TEXTURE_WRAP Sampler::getWrapW() const {
        return m_WrapW;
    }

    SR_COMPARISON_FUNC Sampler::getComparisonFunc() const {
        return m_ComparisonFunc;
    }

    float* Sampler::getBorderColor() {
        return m_BorderColor;
    }

    float Sampler::getMinLOD() const {
        return m_MinLOD;
    }

    float Sampler::getMaxLOD() const {
        return m_MaxLOD;
    }

    float Sampler::getLODBias() const {
        return m_LODBias;
    }

}