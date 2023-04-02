#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_CORE_API ColorAttachmentDesc{
        uint32 width                    = SR_DEFAULT_WIDTH;
        uint32 height                   = SR_DEFAULT_HEIGHT;
        float clearColor[4]             = {0.1f, 0.2f, 0.3f, 1.0f};
        SR_TEXTURE_DATA_FORMAT format   = SR_TEXTURE_DATA_RGBA_F32;
        bool enableShaderRead           = true; // enables sampling from this attachment in shaders
        bool enableAutoResize           = true; // whenever the window is resized, this attachment will be resized as well
    };

    class SR_CORE_API ColorAttachment{
    public:
        explicit ColorAttachment(const ColorAttachmentDesc& desc);

        virtual ~ColorAttachment();

        void setClearColor(float r, float g, float b, float a);

        virtual void bind() = 0;

        virtual void bindShaderResource(uint32 slot) = 0;

        virtual void clear() = 0;

        virtual void setSize(uint32 width, uint32 height) = 0;

        [[nodiscard]] virtual Resource<Image> getData() = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_DATA_FORMAT getFormat() const;

        [[nodiscard]] const float* getClearColor() const;

        [[nodiscard]] bool shaderReadEnabled() const;

    protected:
        uint32 m_Width;
        uint32 m_Height;
        float m_ClearColor[4];
        SR_TEXTURE_DATA_FORMAT m_Format;
        bool m_EnableShaderRead;

    private:

        friend class FrameBuffer;

        void onResize(uint32 width, uint32 height); // called by the framebuffer when its owning window is resized

    private:
        bool m_EnableAutoResize;
    };

}