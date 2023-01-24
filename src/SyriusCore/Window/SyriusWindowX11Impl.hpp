#pragma once

#include "../../../include/SyriusCore/Window/SyriusWindow.hpp"
#include "../../../include/SyriusCore/Core/PlatformDetection.hpp"
#include "../Core/DebugMacros.hpp"
#include "../Core/PlatformAPIX11Impl.hpp"
#include "X11Utils.hpp"
#include "../Context/OpenGL/Internal/GlxContext.hpp"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    class SyriusWindowX11Impl: public SyriusWindow{
    public:

        SyriusWindowX11Impl(const WindowDesc& desc, Display* display, CoreCommand* coreCommand);

        SyriusWindowX11Impl(const SyriusWindowX11Impl&) = delete;

        ~SyriusWindowX11Impl() override;

        void close() override;

        void setPosition(int32 posX, int32 posY) override;

        void resize(uint32 newWidth, uint32 newHeight) override;

        void requestFocus() override;

        void enableFullscreen() override;

        void disableFullscreen() override;

        void show() override;

        void hide() override;

        void setTitle(const std::string& newTitle) override;

        void setIcon(const std::string& imagePath, bool flipVertically, uint32 icons) override;

        void setIcon(Image* image, uint32 icons) override;

        void pollEvents() override;

        void setMousePosition(int32 mousePosX, int32 mousePosY) override;

        int32 getMousePositionX() override;

        int32 getMousePositionY() override;

        void hideMouse() override;

        void showMouse() override;

        void grabMouse() override;

        void releaseMouse() override;

        void centerWindow() override;

        void centerMouse() override;

        std::string openFileDialog(const std::string& filter) override;

        std::string saveFileDialog(const std::string& fileName, const std::string& filter) override;

        ResourceView<Context> createContext(ContextDesc& desc) override;

    private:

        XVisualInfo* selectBestVisual();

        void setWindowProtocols();

        void setWindowStyles(SR_WINDOW_STYLE style);

    private:
        Display* m_Display;

        Window m_Window;

        GLXFBConfig m_BestFBConfig;

    };

}

#endif
