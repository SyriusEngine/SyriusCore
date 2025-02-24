#pragma once

#include "../../../include/SyriusCore/Window/SyriusWindow.hpp"
#include "../Context/D3D11/D3D11Context.hpp"

#include "Win32Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    struct SavedWindowInfo{
        bool m_IsMaximized = false;
        LONG m_Style;
        LONG m_ExStyle;
        RECT m_Rect;
    };

    static const wchar_t* s_SyriusWindowClass = L"SYRIUS_CORE_API";

    class SyriusWindowWin32Impl: public SyriusWindow{
    public:
        explicit SyriusWindowWin32Impl(const WindowDesc& desc);

        SyriusWindowWin32Impl(const SyriusWindowWin32Impl&) = delete;

        ~SyriusWindowWin32Impl() override;

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

        [[nodiscard]] const HWND& getHwnd() const;

    private:

        static LRESULT CALLBACK windowEventProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        void handleEvent(UINT msg, WPARAM wparam, LPARAM lparam);

        void mouseTracker(bool enableTracking) const;

  static       void registerWindowClass();

        static void unregisterWindowClass();

        static void setProcessDPIAwareness();

    private:
        static u32 m_WindowCount;

        HWND m_Hwnd;
        LONG_PTR m_Callback;
        HICON m_Icon;

        SavedWindowInfo m_SavedWindowInfo;

        bool m_CaptureMouseAndKeyboardEvents = false;

    };
}

#endif

