#pragma once

#include "../../../../include/SyriusCore/Context/CubeColorAttachment.hpp"
#include "GlUtils.hpp"

namespace Syrius{

    class GlCubeColorAttachment : public CubeColorAttachment{
    public:
        GlCubeColorAttachment(const CubeColorAttachmentDesc& desc, const Resource<DeviceLimits>& deviceLimits, uint32 framebufferID, uint32 baseAttachmentID);

        ~GlCubeColorAttachment() override;

        void bind() override;

        void bindShaderResource(uint32 slot) override;

        void clear() override;

        void onResize(uint32 width, uint32 height) override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_TextureID;
        const uint32 m_FrameBufferID;
        const uint32 m_BaseAttachmentID;

        GLint m_InternalFormat;
        GLenum m_GlFormat;
        GLenum m_GlDataType;
    };

}