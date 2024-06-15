#pragma once

#include "CubeMapLayout.hpp"

namespace Syrius{

    class Texture2D;
    class ColorAttachment;

    class SR_CORE_API CubeMap{
    public:
        CubeMap() = delete;

        CubeMap(const ResourceView<CubeMapLayout>& desc, const Resource<DeviceLimits>& deviceLimits);

        virtual ~CubeMap();

        virtual void bind() = 0;

        virtual void bindShaderResource(uint32 slot) = 0;

        virtual void copyFrom(const ResourceView<Texture2D>& other, SR_CUBEMAP_FACE destinationFace) = 0;

        virtual void copyFrom(const ResourceView<ColorAttachment>& other, SR_CUBEMAP_FACE destinationFace) = 0;

        virtual Resource<Image> getData(SR_CUBEMAP_FACE destinationFace) = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_FORMAT getFormat() const;

    protected:
        const Resource<DeviceLimits>& m_DeviceLimits;

        const uint32 m_Width;
        const uint32 m_Height;
        const SR_TEXTURE_FORMAT m_Format;


    };

}