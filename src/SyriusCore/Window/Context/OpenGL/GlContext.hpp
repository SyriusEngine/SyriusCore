#pragma once

#include "../../../../../include/SyriusCore/Window/Context/Context.hpp"
#include "../../../Core/CoreCommand.hpp"

namespace Syrius{

    class GlContext: public Context{
    public:

        ~GlContext() override;

        std::string getAPIVersion() override;

        std::string getDeviceName() override;

        std::string getShadingLanguageVersion() override;

        int32 getMaxFramebufferWidth() override;

        int32 getMaxFramebufferHeight() override;

        int32 getMaxFramebufferTextureAttachments() override;

        int32 getMaxTextureSlots() override;

        int32 getMaxTexture2DSize() override;

        int32 getMaxConstantBufferSize() override;

        int32 getMaxDepthBufferBits() override;

    protected:
        GlContext();
        
        void initGl();
    };

}
