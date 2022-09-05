#pragma once

#include "Utils.hpp"
#include "../Utils/Image.hpp"

namespace Syrius{

    struct SR_API FrameBufferDesc{
        uint32 m_Width = 800;
        uint32 m_Height = 600;
        int32 m_XPos = 0;
        int32 m_YPos = 0;
        float m_MinDepth = 0.0f;
        float m_MaxDepth = 1.0f;
        float m_ClearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        bool m_EnableDepthTest = false;
        SR_DEPTH_FUNC m_DepthFunc = SR_DEPTH_FUNC_LESS;
        uint32 m_NumColorAttachments = 1;
        std::vector<SR_TEXTURE_DATA_FORMAT> m_ColorAttachments = {SR_TEXTURE_DATA_FORMAT_RGBA_8};
    };

    class SR_API FrameBuffer{
    public:
        explicit FrameBuffer(const FrameBufferDesc& desc);

        virtual ~FrameBuffer();

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual void setClearColor(float red, float green, float blue, float alpha) = 0;

        virtual void setPosition(int32 xPos, int32 yPos) = 0;

        virtual void setDepthFunc(SR_DEPTH_FUNC func) = 0;

        virtual void onResize(uint32 width, uint32 height) = 0;

        virtual void enableDepthTest() = 0;

        virtual void disableDepthTest() = 0;

        virtual void clear() = 0;

        virtual void bindColorAttachment(uint32 slot, uint32 attachmentIndex) = 0;

        virtual Image* readColorAttachment(uint32 attachmentIndex) = 0;

        virtual uint64 getColorAttachmentIdentifier(uint32 attachmentIndex) = 0;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] int32 getXPos() const;

        [[nodiscard]] int32 getYPos() const;

        [[nodiscard]] float getMinDepth() const;

        [[nodiscard]] float getMaxDepth() const;

        [[nodiscard]] bool isDepthTestEnabled() const;

        [[nodiscard]] SR_DEPTH_FUNC getDepthFunc() const;

    protected:
        uint32 m_Width;
        uint32 m_Height;
        int32 m_XPos;
        int32 m_YPos;

        float m_MinDepth;
        float m_MaxDepth;
        bool m_EnableDepthTest;
        SR_DEPTH_FUNC m_DepthFunc;

        float m_ClearColor[4];

    };

}

