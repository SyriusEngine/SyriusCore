#pragma once

#include "../../../../include/SyriusCore/Context/CubeMap.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlCubeMap : public CubeMap{
    public:
        GlCubeMap(const ResourceView<CubeMapLayout>& desc, const Resource<DeviceLimits>& deviceLimits);

        ~GlCubeMap() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:

        static bool checkFormatSupported(GLint toCheck);

    private:
        uint32 m_TextureID;

        GLenum m_GlFormat;
        GLint m_GlInternalFormat;
        GLenum m_GlDataType;
    };
}