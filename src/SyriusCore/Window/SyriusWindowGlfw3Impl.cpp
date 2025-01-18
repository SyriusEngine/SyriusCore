#include "SyriusWindowGlfw3Impl.hpp"

#include "../Utils/CoreLogger.hpp"
#include "Glfw3Utils.hpp"

#if !defined(SR_PLATFORM_WIN64)

namespace Syrius {

    u64 SyriusWindowGlfw3Impl::m_GlfwWindowCount = 0;

    SyriusWindowGlfw3Impl::SyriusWindowGlfw3Impl(const WindowDesc &desc):
    SyriusWindow(desc){
        initGlfw();
        setWindowStyleHints(desc.style);

        createGlfwWindow();

        SR_POSTCONDITION(m_Window != nullptr, "Window creation failed!");
        SR_POSTCONDITION(glfwGetWindowUserPointer(m_Window) == this, "Failed to store pointer to client window!");
    }

    SyriusWindowGlfw3Impl::~SyriusWindowGlfw3Impl() {
        destroyGlfwWindow();
        if (m_Context) {
            if (m_UseImGui){
                destroyImGuiContext();
            }
            m_Context.reset(); // Destroy context BEFORE glfwTerminate
        }
        terminateGlfw();
    }

    Event SyriusWindowGlfw3Impl::getNextEvent() {
        std::lock_guard lock(m_EventQueueMutex);

        auto event = m_EventQueue[0];
        m_EventQueue.pop_front();
        return event;
    }


    void SyriusWindowGlfw3Impl::close() {
        glfwWindowShouldClose(m_Window);
    }

    void SyriusWindowGlfw3Impl::setPosition(const i32 posX, const i32 posY) {
        glfwSetWindowPos(m_Window, posX, posY);
        m_PosX = posX;
        m_PosY = posY;
    }

    void SyriusWindowGlfw3Impl::resize(const u32 newWidth, const u32 newHeight) {
        glfwSetWindowSize(m_Window, static_cast<int>(newWidth), static_cast<int>(newHeight));
    }

    void SyriusWindowGlfw3Impl::requestFocus() {
        glfwRequestWindowAttention(m_Window);
    }

    void SyriusWindowGlfw3Impl::enableFullscreen() {

    }

    void SyriusWindowGlfw3Impl::disableFullscreen() {

    }

    void SyriusWindowGlfw3Impl::show() {
        glfwShowWindow(m_Window);
        m_Focused = true;
    }

    void SyriusWindowGlfw3Impl::hide() {
        glfwHideWindow(m_Window);
        m_Focused = false;
    }

    void SyriusWindowGlfw3Impl::setTitle(const std::string &newTitle) {
        glfwSetWindowTitle(m_Window, newTitle.c_str());
        m_Title = newTitle;
    }

    void SyriusWindowGlfw3Impl::setIcon(const ImageFileDesc &desc, u32 icons) {

    }

    void SyriusWindowGlfw3Impl::setIcon(const UP<Image> &image, u32 icons) {

    }

    void SyriusWindowGlfw3Impl::pollEvents() {
        glfwPollEvents();
    }

    void SyriusWindowGlfw3Impl::setMousePosition(i32 mousePosX, i32 mousePosY) {

    }

    i32 SyriusWindowGlfw3Impl::getMousePositionX() {
        return 0;
    }

    i32 SyriusWindowGlfw3Impl::getMousePositionY() {
        return 0;
    }

    void SyriusWindowGlfw3Impl::hideMouse() {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void SyriusWindowGlfw3Impl::showMouse() {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void SyriusWindowGlfw3Impl::grabMouse() {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void SyriusWindowGlfw3Impl::releaseMouse() {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void SyriusWindowGlfw3Impl::centerWindow() {
        if (m_Fullscreen) {
            return;
        }
        const auto monitor = glfwGetPrimaryMonitor();
        const auto vidMode = glfwGetVideoMode(monitor);
        const auto xPos = static_cast<i32>((vidMode->width - m_Width) / 2);
        const auto yPos = static_cast<i32>((vidMode->height - m_Height) / 2);
        setPosition(xPos, yPos);
    }

    void SyriusWindowGlfw3Impl::centerMouse() {
        const i32 mouseX = static_cast<i32>(m_Width / 2);
        const i32 mouseY = static_cast<i32>(m_Height / 2);
        setMousePosition(mouseX, mouseY);
    }

    std::string SyriusWindowGlfw3Impl::openFileDialog(const std::string &filter) {
        SR_LOG_WARNING("SyriusWindowGlfw3Impl", "Not Implemented");
        return {};
    }

    std::string SyriusWindowGlfw3Impl::saveFileDialog(const std::string &fileName, const std::string &filter) {
        SR_LOG_WARNING("SyriusWindowGlfw3Impl", "Not Implemented");
        return {};
    }

    ResourceView<Context> SyriusWindowGlfw3Impl::createContext(ContextDesc &desc) {
        /*
         * Since GLFW does not allow for context creation AFTER window creation.
         * We simply destroy the window and recreate it with the new context.
         * This is not ideal, but it is the only way to do it with GLFW.
         * Can cause flickering.
         */
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(m_Window, &width, &height);
        desc.backBufferWidth = desc.backBufferWidth == 0 ? width : desc.backBufferWidth;
        desc.backBufferHeight = desc.backBufferHeight == 0 ? height : desc.backBufferHeight;
        switch (desc.api) {
            case SR_API_OPENGL: {
                createOpenGLContext(desc);
                break;
            }
            default: {
                SR_LOG_WARNING("SyriusWindowGlfw3Impl", "cannot create context: unsupported API (%i), fallback to OpenGL!", desc.api);
                createOpenGLContext(desc);
            }
        }
        return createResourceView(m_Context);
    }

    void SyriusWindowGlfw3Impl::createGlfwWindow() {
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        SR_LOG_THROW_IF_FALSE(m_Window != nullptr, "SyriusWindowGlfw3Impl", "Failed to create window: %s", m_Title.c_str());
        m_Open = true;
        glfwShowWindow(m_Window);
        m_Focused = true;

        /*
         * Store a pointer to our window on the GLFW3 side.
         * This pointer is used to forward events to the correct SyriusWindow.
         */
        glfwSetWindowUserPointer(m_Window, this);

        // Fire WindowOpenedEvent
        std::lock_guard lock(m_EventQueueMutex);
        const WindowOpenedEvent event;
        dispatchEvent(event);

        // Then, set all window callbacks
        glfwSetWindowPosCallback(m_Window, positionCallback);
        glfwSetWindowSizeCallback(m_Window, resizeCallback);
        glfwSetWindowCloseCallback(m_Window, closeCallback);
        glfwSetWindowRefreshCallback(m_Window, refreshCallback);
        glfwSetWindowFocusCallback(m_Window, focusCallback);

        // Next, all input callbacks
        glfwSetKeyCallback(m_Window, keyCallback);
        glfwSetCharCallback(m_Window, keyTypedCallback);
        glfwSetMouseButtonCallback(m_Window, mouseButtonCallback);
        glfwSetCursorPosCallback(m_Window, mousePositionCallback);
        glfwSetScrollCallback(m_Window, mouseScrollCallback);
    }

    void SyriusWindowGlfw3Impl::destroyGlfwWindow() {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr; // Sanity
    }

    void SyriusWindowGlfw3Impl::createOpenGLContext(ContextDesc& desc) {
        SR_PRECONDITION(desc.api == SR_API_OPENGL, "SyriusWindowGlfw3Impl", "Cannot create OpenGL context for api: %i", desc.api);

        // To create a new OpenGL context with the desired parameters from desc.
        // We first destroy the old window, set our window creation hints
        // Which includes OpenGL context hints, then create the window followed with our context creation.
        destroyGlfwWindow();
        Glfw3glContext::setGlfw3glContextHints(desc);
        createGlfwWindow();
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(m_Window, &width, &height);
        desc.backBufferWidth = desc.backBufferWidth == 0 ? width : desc.backBufferWidth;
        desc.backBufferHeight = desc.backBufferHeight == 0 ? height : desc.backBufferHeight;
        m_Context = createUP<Glfw3glContext>(m_Window, desc);
    }

    void SyriusWindowGlfw3Impl::initGlfw() {
        if (m_GlfwWindowCount == 0) {
            SR_LOG_THROW_IF_FALSE(glfwInit(), "SyriusWindowGlfw3Impl", "Failed to initialize GLFW");

            glfwSetErrorCallback(CoreLogger::glfwCallback);

            // Log version
            int major;
            int minor;
            int revision;
            glfwGetVersion(&major, &minor, &revision);
            SR_LOG_INFO("SyriusWindowGlfw3Impl", "GLFW3 initialised with version: %i.%i.%i", major, minor, revision);
        }
        m_GlfwWindowCount++;
    }

    void SyriusWindowGlfw3Impl::terminateGlfw() {
        m_GlfwWindowCount--;
        if (m_GlfwWindowCount == 0) {
            glfwTerminate();
        }
    }

    void SyriusWindowGlfw3Impl::setWindowStyleHints(const SR_WINDOW_STYLE style) {
        if (style == SR_WINDOW_STYLE_DEFAULT) {
            return; // don't set anything since this defaults to GLFW's default style
        }
        if (style == SR_WINDOW_STYLE_POPUP) {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            return;
        }

        if (!(style & SR_WINDOW_STYLE_RESIZE)) {
            glfwWindowHint(	GLFW_RESIZABLE, GLFW_FALSE);
        }
    }


    void SyriusWindowGlfw3Impl::positionCallback(GLFWwindow *window, const int xpos, const int ypos) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        const WindowMovedEvent event(xpos, ypos);
        syriusWindow->dispatchEvent(event);
    }

    void SyriusWindowGlfw3Impl::resizeCallback(GLFWwindow *window, int width, int height) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        syriusWindow->m_Width = width;
        syriusWindow->m_Height = height;
        const WindowResizedEvent event(width, height);
        syriusWindow->dispatchEvent(event);
    }

    void SyriusWindowGlfw3Impl::closeCallback(GLFWwindow *window) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        const WindowClosedEvent event;
        syriusWindow->dispatchEvent(event);
        syriusWindow->m_Open = false;
    }

    void SyriusWindowGlfw3Impl::refreshCallback(GLFWwindow *window) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        const WindowRefreshedEvent event;
        syriusWindow->dispatchEvent(event);
    }

    void SyriusWindowGlfw3Impl::focusCallback(GLFWwindow *window, const int focused) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        if (focused == GLFW_TRUE) {
            const WindowGainedFocusEvent event;
            syriusWindow->dispatchEvent(event);
        }
        else {
            const WindowLostFocusEvent event;
            syriusWindow->dispatchEvent(event);
        }
    }

    void SyriusWindowGlfw3Impl::keyCallback(GLFWwindow *window, const int key, int scancode, const int action, int mods) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        switch (action) {
            case GLFW_PRESS: {
                const KeyPressedEvent event(translateGlfwKey(key));
                syriusWindow->dispatchEvent(event);
            }
            case GLFW_RELEASE: {
                const KeyReleasedEvent event(translateGlfwKey(key));
                syriusWindow->dispatchEvent(event);
            }
            default: break;
        }
    }

    void SyriusWindowGlfw3Impl::keyTypedCallback(GLFWwindow *window, const unsigned int codepoint) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        const KeyTypedEvent event(static_cast<char>(codepoint));
        syriusWindow->dispatchEvent(event);
    }

    void SyriusWindowGlfw3Impl::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        switch (action) {
            case GLFW_PRESS: {
                const MouseButtonPressedEvent event(translateGlfwMouseButton(button));
                syriusWindow->dispatchEvent(event);
            }
            case GLFW_RELEASE: {
                const MouseButtonReleasedEvent event(translateGlfwMouseButton(button));
                syriusWindow->dispatchEvent(event);
            }
            default: break;
        }
    }

    void SyriusWindowGlfw3Impl::mousePositionCallback(GLFWwindow *window, double xpos, double ypos) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        const MouseMovedEvent event(static_cast<i32>(xpos), static_cast<i32>(ypos));
        syriusWindow->dispatchEvent(event);
    }

    void SyriusWindowGlfw3Impl::mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
        const auto syriusWindow = static_cast<SyriusWindowGlfw3Impl*>(glfwGetWindowUserPointer(window));
        std::lock_guard lock(syriusWindow->m_EventQueueMutex);

        const MouseScrolledEvent event(static_cast<i32>(xoffset), static_cast<i32>(yoffset));
        syriusWindow->dispatchEvent(event);
    }

}

#endif