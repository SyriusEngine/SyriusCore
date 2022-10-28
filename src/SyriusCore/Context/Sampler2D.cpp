#include "../../../include/SyriusCore/Context/Sampler2D.hpp"

namespace Syrius{

    Sampler2D::Sampler2D(const Sampler2DDesc &desc) {

    }

    Sampler2D::~Sampler2D() {

    }

    const Sampler2DDesc &Sampler2D::getDesc() const {
        return m_Desc;
    }
}