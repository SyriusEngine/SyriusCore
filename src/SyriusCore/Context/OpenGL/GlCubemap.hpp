#pragma once

#include "../../../../include/SyriusCore/Context/Cubemap.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlCubemap : public Cubemap{
    public:
        GlCubemap(const CubemapDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        GlCubemap(const CubemapImageDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        ~GlCubemap() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:

        static bool checkFormatSupported(GLint toCheck);

        void determineFormats();

        void createCubemap(const void* faces[6]);

    private:
        uint32 m_TextureID;

        GLenum m_GlFormat;
        GLint m_GlInternalFormat;
        GLenum m_GlDataType;
    };
}