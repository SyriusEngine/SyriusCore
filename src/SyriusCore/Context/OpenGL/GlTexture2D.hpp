#pragma once

#include "../../../../include/SyriusCore/Context/Texture2D.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlTexture2D : public Texture2D{
    public:
        GlTexture2D(const Texture2DDesc& desc, const UP<DeviceLimits>& deviceLimits);

        GlTexture2D(const Texture2DImageDesc& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlTexture2D() override;

        void bind() override;

        void bindShaderResource(u32 slot) override;

        void setData(const void* data, u32 x, u32 y, u32 width, u32 height) override;

        void copyFrom(const ResourceView<Texture2D>& other) override;

        void copyFrom(const ResourceView<ColorAttachment>& other) override;

        UP<Image> getData() override;

        [[nodiscard]] u64 getIdentifier() const override;

    private:

        void createTexture(const void* data);

    private:
        u32 m_TextureID;
        u32 m_PixelUnpackBuffer;

        GLenum m_GlChannelFormat;
        GLint m_GlInternalFormat;
        GLenum m_GlDataType;
    };
}

