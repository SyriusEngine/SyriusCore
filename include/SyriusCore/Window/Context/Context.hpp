#pragma once

#include "../../Core/SyriusCoreInclude.hpp"
#include "../../../../src/SyriusCore/Core/DebugMacros.hpp"

namespace Syrius{

    class SR_API Context{
    public:

        Context() = delete;

        Context(const Context&) = delete;

        virtual ~Context();

        virtual void makeCurrent() = 0;

        virtual void releaseCurrent() = 0;

        virtual void swapBuffers() = 0;

        virtual void setVerticalSynchronisation(bool enable) = 0;

        virtual void createImGuiContext() = 0;

        virtual void destroyImGuiContext() = 0;

        virtual void onImGuiBegin() = 0;

        virtual void onImGuiEnd() = 0;

        [[nodiscard]] SR_SUPPORTED_API getType() const;

        bool isVerticalSyncEnabled() const;

    protected:

        explicit Context(SR_SUPPORTED_API type);

    protected:

        bool m_VerticalSync;

    private:

        SR_SUPPORTED_API m_Type;

    };

}