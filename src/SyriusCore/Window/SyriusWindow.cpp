#include "../../../include/SyriusCore/Window/SyriusWindow.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    SyriusWindow::SyriusWindow(const WindowDesc &desc):
    m_PosX(desc.xPos),
    m_PosY(desc.yPos),
    m_Width(desc.width),
    m_Height(desc.height),
    m_Title(desc.title),
    m_Open(false),
    m_Focused(false),
    m_Moving(false),
    m_Resizing(false),
    m_MouseGrabbed(false),
    m_MouseVisible(true),
    m_Fullscreen(false),
    m_MouseInside(false),
    m_KeyRepeat(true),
    m_Context(nullptr){

    }

    i32 SyriusWindow::getPosX() const {
        return m_PosX;
    }

    i32 SyriusWindow::getPosY() const {
        return m_PosY;
    }

    u32 SyriusWindow::getWidth() const {
        return m_Width;
    }

    u32 SyriusWindow::getHeight() const {
        return m_Height;
    }

    const std::string &SyriusWindow::getTitle() const {
        return m_Title;
    }

    bool SyriusWindow::isOpen() const {
        return m_Open;
    }

    bool SyriusWindow::isFocused() const {
        return m_Focused;
    }

    bool SyriusWindow::isFullscreen() const {
        return m_Fullscreen;
    }

    bool SyriusWindow::hasEvent() const {
        return !m_EventQueue.empty();
    }

    void SyriusWindow::dispatchEvent(const Event &event) {
        m_EventQueue.push_back(event);
    }

    void SyriusWindow::destroyContext() {
        m_Context.reset();
    }

    Event SyriusWindow::getNextEvent() {
        auto event = m_EventQueue.front();
        m_EventQueue.pop_front();
        return event;
    }

}