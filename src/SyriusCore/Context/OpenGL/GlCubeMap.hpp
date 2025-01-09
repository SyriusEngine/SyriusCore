#pragma once

#include "../../../../include/SyriusCore/Context/CubeMap.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlCubeMap : public CubeMap{
    public:
        GlCubeMap(const ResourceView<CubeMapLayout>& desc, const UP<DeviceLimits>& deviceLimits);

        ~GlCubeMap() override;

        void bind() override;

        void bindShaderResource(u32 slot) override;

        void copyFrom(const ResourceView<Texture2D>& other, SR_CUBEMAP_FACE destinationFace) override;

        void copyFrom(const ResourceView<ColorAttachment>& other, SR_CUBEMAP_FACE destinationFace) override;

        UP<Image> getData(SR_CUBEMAP_FACE destinationFace) override;

        [[nodiscard]] u64 getIdentifier() const override;

    private:

        static bool checkFormatSupported(GLint toCheck);

    private:
        u32 m_TextureID;

        GLenum m_GlChannelFormat;
        GLint m_GlInternalFormat;
        GLenum m_GlDataType;
    };
}