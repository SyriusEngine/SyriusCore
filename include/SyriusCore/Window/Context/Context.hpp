#pragma once

#include "../../Core/SyriusCoreInclude.hpp"
#include "../../../../src/SyriusCore/Core/DebugMacros.hpp"

namespace Syrius{

    class SR_API Context{
    public:

        Context() = delete;

        Context(const Context&) = delete;

        virtual ~Context();

        virtual void swapBuffers() = 0;

        virtual void setVerticalSynchronisation(bool enable) = 0;

        virtual void createImGuiContext() = 0;

        virtual void destroyImGuiContext() = 0;

        virtual void onImGuiBegin() = 0;

        virtual void onImGuiEnd() = 0;

        [[nodiscard]] SR_SUPPORTED_API getType() const;

        [[nodiscard]] bool isVerticalSyncEnabled() const;

        virtual std::string getAPIVersion() = 0;

        virtual std::string getDeviceName() = 0;

        virtual std::string getShadingLanguageVersion() = 0;

        virtual int32 getMaxFramebufferWidth() = 0;

        virtual int32 getMaxFramebufferHeight() = 0;

        virtual int32 getMaxFramebufferTextureAttachments() = 0;

        virtual int32 getMaxTextureSlots() = 0;

        virtual int32 getMaxTexture2DSize() = 0;

        virtual int32 getMaxConstantBufferSize() = 0;

        virtual int32 getMaxDepthBufferBits() = 0;

    protected:

        explicit Context(SR_SUPPORTED_API type);

    protected:

        bool m_VerticalSync;

    private:

        SR_SUPPORTED_API m_Type;

    };

}