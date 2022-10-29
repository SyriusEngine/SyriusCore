#include "../../../include/SyriusCore/Context/Sampler.hpp"

namespace Syrius{

    Sampler::Sampler(const Sampler2DDesc &desc) {

    }

    Sampler::~Sampler() {

    }

    const Sampler2DDesc &Sampler::getDesc() const {
        return m_Desc;
    }
}