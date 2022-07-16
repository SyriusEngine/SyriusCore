#include "../../../../include/SyriusCore/Window/SyriusWindow.hpp"

namespace Syrius{

    uint32 SyriusWindow::m_ImGuiInstances = 0;

    SyriusWindow::SyriusWindow(const WindowDesc &desc)
    : m_PosX(desc.m_PosX),
      m_PosY(desc.m_PosY),
      m_Width(desc.m_Width),
      m_Height(desc.m_Height),
      m_Title(desc.m_Title),
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
        delete m_Context;

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

    void SyriusWindow::requestImGuiContext() {
        SR_CORE_PRECONDITION(m_Context, "A valid context must be created in order to create an ImGui context")
        SR_CORE_PRECONDITION(!m_UseImGui, "There exists already an ImGui context")

        m_Context->createImGuiContext();
        m_UseImGui = true;
        m_ImGuiInstances++;
    }

    void SyriusWindow::releaseImGuiContext() {
        SR_CORE_PRECONDITION(m_Context, "A valid context must be created in order to destroy an ImGui context")
        SR_CORE_PRECONDITION(m_UseImGui, "There must be an ImGui context created")

        m_Context->destroyImGuiContext();
        m_UseImGui = false;
        m_ImGuiInstances--;
    }

    void SyriusWindow::onImGuiBegin() {
        SR_CORE_PRECONDITION(m_Context, "A valid context must be created in order to destroy an ImGui context")
        SR_CORE_PRECONDITION(m_UseImGui, "There must be an ImGui context created")

        m_Context->onImGuiBegin();
    }

    void SyriusWindow::onImGuiEnd() {
        SR_CORE_PRECONDITION(m_Context, "A valid context must be created in order to destroy an ImGui context")
        SR_CORE_PRECONDITION(m_UseImGui, "There must be an ImGui context created")

        m_Context->onImGuiEnd();
    }

    void SyriusWindow::dispatchEvent(const Event &event) {
        m_EventQueue.push_back(event);
    }

    void SyriusWindow::update() {
        SR_CORE_PRECONDITION(m_Context, "A valid context must be created in order to destroy an ImGui context")

        m_Context->swapBuffers();
    }

    void SyriusWindow::releaseContext() {
        SR_CORE_PRECONDITION(m_Context, "A valid context must be created in order to destroy the context")
        SR_CORE_PRECONDITION(!m_UseImGui, "ImGui is still using this context, cannot destroy the context")

        delete m_Context;
        m_Context = nullptr;
    }

}