#pragma once

#include "Event/Event.hpp"
#include "Event/KeyboardEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Event/WindowEvent.hpp"

#include "../Utils/Image.hpp"

#include "../Context/Context.hpp"

typedef enum SR_WINDOW_STYLE {
    SR_WINDOW_STYLE_POPUP       = 0x01,   // 0000 0001
    SR_WINDOW_STYLE_TITLEBAR    = 0x02,    // 0000 0010
    SR_WINDOW_STYLE_RESIZE      = 0x04,    // 0000 0100
    SR_WINDOW_STYLE_CLOSE       = 0x08,    // 0000 1000
    SR_WINDOW_STYLE_FILE_DROP   = 0x10,    // 0001 0000
    SR_WINDOW_STYLE_SUNKEN_EDGE = 0x20,    // 0010 0000
    SR_WINDOW_STYLE_RAISED_EDGE = 0x40,    // 0100 0000
    SR_WINDOW_STYLE_DEFAULT     = 0x1e    // 0001 1110

} SR_WINDOW_STYLE;

typedef enum SR_WIDOW_ICON {
    SR_WINDOW_ICON_NONE     = 0x00,
    SR_WINDOW_ICON_SMALL    = 0x01,
    SR_WINDOW_ICON_BIG      = 0x02

} SR_WINDOW_ICON;

namespace Syrius{

    struct WindowDesc{
        int32 xPos            = 200;
        int32 yPos            = 200;
        uint32 width          = SR_DEFAULT_WIDTH;
        uint32 height         = SR_DEFAULT_HEIGHT;
        std::string title;
        SR_WINDOW_STYLE style = SR_WINDOW_STYLE_DEFAULT;
    };

    class SR_CORE_API SyriusWindow{
    public:
        SyriusWindow() = delete;

        virtual ~SyriusWindow();

        [[nodiscard]] int32 getPosX() const;

        [[nodiscard]] int32 getPosY() const;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        [[nodiscard]] const std::string& getTitle() const;

        [[nodiscard]] bool isOpen() const;

        [[nodiscard]] bool isFocused() const;

        [[nodiscard]] bool isFullscreen() const;

        [[nodiscard]] bool hasEvent() const;

        Event getEvent();

        void createImGuiContext();

        void destroyImGuiContext();

        void onImGuiBegin();

        void onImGuiEnd();

        void destroyContext();

        virtual void close() = 0;

        virtual void setPosition(int32 posX, int32 posY) = 0;

        virtual void resize(uint32 newWidth, uint32 newHeight) = 0;

        virtual void requestFocus() = 0;

        virtual void enableFullscreen() = 0;

        virtual void disableFullscreen() = 0;

        virtual void show() = 0;

        virtual void hide() = 0;

        virtual void setTitle(const std::string& newTitle) = 0;

        virtual void setIcon(const std::string& imagePath, bool flipVertically, uint32 icons) = 0;

        virtual void setIcon(Image* image, uint32 icons) = 0;

        virtual void pollEvents() = 0;

        virtual void setMousePosition(int32 mousePosX, int32 mousePosY) = 0;

        virtual int32 getMousePositionX() = 0;

        virtual int32 getMousePositionY() = 0;

        virtual void hideMouse() = 0;

        virtual void showMouse() = 0;

        virtual void grabMouse() = 0;

        virtual void releaseMouse() = 0;

        virtual void centerWindow() = 0;

        virtual void centerMouse() = 0;

        virtual std::string openFileDialog(const std::string& filter) = 0;

        virtual std::string saveFileDialog(const std::string& fileName, const std::string& filter) = 0;

        virtual ResourceView<Context> createContext(ContextDesc& desc) = 0;

    protected:

        explicit SyriusWindow(const WindowDesc& desc);

        void dispatchEvent(const Event& event);

    protected:
        int32 m_PosX;
        int32 m_PosY;
        uint32 m_Width;
        uint32 m_Height;
        std::string m_Title;
        bool m_Open;
        bool m_Focused;
        bool m_Moving;
        bool m_Resizing;
        bool m_MouseGrabbed;
        bool m_MouseVisible;
        bool m_Fullscreen;
        bool m_MouseInside;
        bool m_KeyRepeat;
        bool m_UseImGui;

        static uint32 m_ImGuiInstances;

        Resource<Context> m_Context;

    private:
        std::deque<Event> m_EventQueue;
    };

}

