#pragma once

#include "../../../../include/SyriusCore/Context/Texture2D.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlTexture2D : public Texture2D{
    public:
        explicit GlTexture2D(const Texture2DDesc& desc);

        ~GlTexture2D() override;

        void bind(uint32_t slot) override;

        void unbind() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_TextureID;
    };
}

