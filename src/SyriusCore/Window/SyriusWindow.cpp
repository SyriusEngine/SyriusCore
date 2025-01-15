#include "../../../include/SyriusCore/Window/SyriusWindow.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    u32 SyriusWindow::m_ImGuiInstances = 0;

    SyriusWindow::SyriusWindow(const WindowDesc &desc)
    : m_PosX(desc.xPos),
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
      m_UseImGui(false),
      m_Context(nullptr){

    }

    SyriusWindow::~SyriusWindow() {
        if (m_UseImGui){
            this->destroyImGuiContext();
        }
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

    void SyriusWindow::createImGuiContext() {
        SR_PRECONDITION(m_Context != nullptr, "A valid context must be created in order to create an ImGui context")
        SR_PRECONDITION(!m_UseImGui, "There exists already an ImGui context")

        m_Context->createImGuiContext();
        m_UseImGui = true;
        m_ImGuiInstances++;
    }

    void SyriusWindow::destroyImGuiContext() {
        SR_PRECONDITION(m_Context != nullptr, "A valid context must be created in order to destroy an ImGui context")
        SR_PRECONDITION(m_UseImGui, "There must be an ImGui context created")

        m_Context->destroyImGuiContext();
        m_UseImGui = false;
        m_ImGuiInstances--;
    }

    void SyriusWindow::onImGuiBegin() {
        SR_PRECONDITION(m_Context != nullptr, "A valid context must be created in order to destroy an ImGui context")
        SR_PRECONDITION(m_UseImGui, "There must be an ImGui context created")

        m_Context->onImGuiBegin();
    }

    void SyriusWindow::onImGuiEnd() {
        SR_PRECONDITION(m_Context != nullptr, "A valid context must be created in order to destroy an ImGui context")
        SR_PRECONDITION(m_UseImGui, "There must be an ImGui context created")

        m_Context->onImGuiEnd();
    }

    void SyriusWindow::dispatchEvent(const Event &event) {
        m_EventQueue.push_back(event);
    }

    void SyriusWindow::destroyContext() {
        SR_LOG_INFO_IF_FALSE(m_UseImGui != 0, "SyriusWindow", "There exists an ImGui context, destroying it");

        if (m_UseImGui){
            this->destroyImGuiContext();
        }
        m_Context.reset();
    }

    Event SyriusWindow::getNextEvent() {
        auto event = m_EventQueue[0];
        m_EventQueue.pop_front();
        return event;
    }

}