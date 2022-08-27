#pragma once

#include "../../../../include/SyriusCore/Context/Texture2D.hpp"

namespace Syrius{

    class GlTexture2D : public Texture2D{
    public:
        GlTexture2D();

        ~GlTexture2D() override;

        void bind(uint32_t slot) override;

        void unbind() override;

        void upload(Image* image) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_TextureID;
    };
}

