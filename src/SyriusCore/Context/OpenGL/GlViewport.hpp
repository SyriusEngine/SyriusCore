#pragma once

#include "../../../../include/SyriusCore/Context/Viewport.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlViewport : public Viewport{
        public:
        explicit GlViewport(const ViewportDesc& desc);

        ~GlViewport() override;

        void bind() override;
    };

}