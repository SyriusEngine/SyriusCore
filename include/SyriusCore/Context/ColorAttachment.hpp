#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_CORE_API ColorAttachmentDesc{
        uint32 m_Width                      = SR_DEFAULT_WIDTH;
        uint32 m_Height                     = SR_DEFAULT_HEIGHT;
        float m_ClearColor[4]               = {0.8f, 0.2f, 0.3f, 1.0f};
        SR_TEXTURE_DATA_FORMAT  m_Format    = SR_TEXTURE_DATA_FORMAT_RGBA_UI8;
    };

    class SR_CORE_API ColorAttachment{
    public:
        explicit ColorAttachment(const ColorAttachmentDesc& desc);

        virtual ~ColorAttachment();

        virtual void clear() = 0;

        virtual void bind(uint32 slot) = 0;

        virtual void onResize(uint32 width, uint32 height) = 0;

        [[nodiscard]] virtual Image* getData() = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] SR_TEXTURE_DATA_FORMAT getFormat() const;

        [[nodiscard]] const float* getClearColor() const;

        void setClearColor(float r, float g, float b, float a);

    protected:
        uint32 m_Width;
        uint32 m_Height;
        SR_TEXTURE_DATA_FORMAT m_Format;
        float m_ClearColor[4];
    };



}
