#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_API ColorAttachmentDesc{
        uint32 m_Width                      = SR_DEFAULT_WIDTH;
        uint32 m_Height                     = SR_DEFAULT_HEIGHT;
        SR_TEXTURE_DATA_FORMAT  m_Format    = SR_TEXTURE_DATA_FORMAT_RGBA_8;
        SR_TEXTURE_FILTER m_MinFilter       = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_FILTER m_MagFilter       = SR_TEXTURE_FILTER_LINEAR;
        SR_TEXTURE_WRAP m_WrapAddressU      = SR_TEXTURE_WRAP_REPEAT;
        SR_TEXTURE_WRAP m_WrapAddressV      = SR_TEXTURE_WRAP_REPEAT;
    };

    class SR_API ColorAttachment{
    public:
        explicit ColorAttachment(const ColorAttachmentDesc& desc);

        virtual ~ColorAttachment();

        virtual void bind(uint32 slot) = 0;

        virtual void onResize(uint32 width, uint32 height) = 0;

        [[nodiscard]] virtual Image* getData() = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

    protected:
        uint32 m_Width;
        uint32 m_Height;
        SR_TEXTURE_DATA_FORMAT m_Format;
    };



}
