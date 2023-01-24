#include "../../../../include/SyriusCore/Window/SyriusWindow.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    uint32 SyriusWindow::m_ImGuiInstances = 0;

    SyriusWindow::SyriusWindow(const WindowDesc &desc, CoreCommand* coreCommand)
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
      m_Context(nullptr),
      m_CoreState(coreCommand){

    }

    SyriusWindow::~SyriusWindow() {
        if (m_UseImGui){
            this->destroyImGuiContext();
        }
    }

    int32 SyriusWindow::getPosX() const {
        return m_PosX;
    }

    int32 SyriusWindow::getPosY() const {
        return m_PosY;
    }

    uint32 SyriusWindow::getWidth() const {
        return m_Width;
    }

    uint32 SyriusWindow::getHeight() const {
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

    Event SyriusWindow::getEvent() {
        auto event = m_EventQueue[0];
        m_EventQueue.pop_front();
        return event;
    }

    void SyriusWindow::createImGuiContext() {
        SR_CORE_PRECONDITION(m_Context.isValid(), "A valid context must be created in order to create an ImGui context")
        SR_CORE_PRECONDITION(!m_UseImGui, "There exists already an ImGui context")

        m_Context->createImGuiContext();
        m_UseImGui = true;
        m_ImGuiInstances++;
    }

    void SyriusWindow::destroyImGuiContext() {
        SR_CORE_PRECONDITION(m_Context.isValid(), "A valid context must be created in order to destroy an ImGui context")
        SR_CORE_PRECONDITION(m_UseImGui, "There must be an ImGui context created")

        m_Context->destroyImGuiContext();
        m_UseImGui = false;
        m_ImGuiInstances--;
    }

    void SyriusWindow::onImGuiBegin() {
        SR_CORE_PRECONDITION(m_Context.isValid(), "A valid context must be created in order to destroy an ImGui context")
        SR_CORE_PRECONDITION(m_UseImGui, "There must be an ImGui context created")

        m_Context->onImGuiBegin();
    }

    void SyriusWindow::onImGuiEnd() {
        SR_CORE_PRECONDITION(m_Context.isValid(), "A valid context must be created in order to destroy an ImGui context")
        SR_CORE_PRECONDITION(m_UseImGui, "There must be an ImGui context created")

        m_Context->onImGuiEnd();
    }

    void SyriusWindow::dispatchEvent(const Event &event) {
        m_EventQueue.push_back(event);
    }

    void SyriusWindow::destroyContext() {

        SR_CORE_MESSAGE_ON_CONDITION(m_UseImGui, "There exists an ImGui context, destroying it");
        if (m_UseImGui){
            this->destroyImGuiContext();
        }
        m_Context.destroy();
    }

}