#pragma once

#include "../../../../include/SyriusCore/Window/SyriusWindow.hpp"
#include "../Context/D3D11/D3D11Context.hpp"

#include "Win32Utils.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    struct SavedWindowInfo{
        bool m_IsMaximized = false;
        LONG m_Style;
        LONG m_ExStyle;
        RECT m_Rect;
    };

    class SyriusWindowWin32Impl: public SyriusWindow{
    public:
        explicit SyriusWindowWin32Impl(const WindowDesc& desc);

        SyriusWindowWin32Impl(const SyriusWindowWin32Impl&) = delete;

        ~SyriusWindowWin32Impl() override;

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

        Context* createContext(const ContextDesc& desc) override;

        [[nodiscard]] const HWND& getHwnd() const;

    private:

        static LRESULT CALLBACK windowEventProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        void handleEvent(UINT msg, WPARAM wparam, LPARAM lparam);

        void mouseTracker(bool enableTracking);

        static void registerClass();

        static void unregisterClass();

    private:
        static uint32 m_WindowCount;

        HWND m_Hwnd;
        LONG_PTR m_Callback;
        HICON m_Icon;

        SavedWindowInfo m_SavedWindowInfo;

        bool m_CaptureMouseAndKeyboardEvents = false;

    };
}

#endif

