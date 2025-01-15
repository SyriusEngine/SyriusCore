#pragma once

#include "../../../include/SyriusCore/Window/SyriusWindow.hpp"

#include <GLFW/glfw3.h>

#if !defined(SR_PLATFORM_WIN64)

namespace Syrius {

    class SyriusWindowGlfw3Impl: public SyriusWindow {
    public:
        explicit SyriusWindowGlfw3Impl(const WindowDesc& desc);

        ~SyriusWindowGlfw3Impl() override;

        void close() override;

        void setPosition(i32 posX, i32 posY) override;

        void resize(u32 newWidth, u32 newHeight) override;

        void requestFocus() override;

        void enableFullscreen() override;

        void disableFullscreen() override;

        void show() override;

        void hide() override;

        void setTitle(const std::string& newTitle) override;

        void setIcon(const ImageFileDesc& desc, u32 icons) override;

        void setIcon(const UP<Image>& image, u32 icons) override;

        void pollEvents() override;

        void setMousePosition(i32 mousePosX, i32 mousePosY) override;

        i32 getMousePositionX() override;

        i32 getMousePositionY() override;

        void hideMouse() override;

        void showMouse() override;

        void grabMouse() override;

        void releaseMouse() override;

        void centerWindow() override;

        void centerMouse() override;

        std::string openFileDialog(const std::string& filter) override;

        std::string saveFileDialog(const std::string& fileName, const std::string& filter) override;

        ResourceView<Context> createContext(ContextDesc& desc) override;

    };

}

#endif

