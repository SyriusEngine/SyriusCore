#pragma once

#include <mutex>

#include "../../../include/SyriusCore/Window/SyriusWindow.hpp"
#include "../Utils/PlatformInclude.hpp"

#if !defined(SR_PLATFORM_WIN64)

namespace Syrius {

    class SyriusWindowGlfw3Impl: public SyriusWindow {
    public:
        explicit SyriusWindowGlfw3Impl(const WindowDesc& desc);

        ~SyriusWindowGlfw3Impl() override;

        Event getNextEvent() override;

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

    private:

        void createGlfwWindow();

        void destroyGlfwWindow();

        static void initGlfw();

        static void terminateGlfw();

        static void setWindowStyleHints(SR_WINDOW_STYLE style);

        static void positionCallback(GLFWwindow* window, int xpos, int ypos);

        static void resizeCallback(GLFWwindow* window, int width, int height);

        static void closeCallback(GLFWwindow* window);

        static void refreshCallback(GLFWwindow* window);

        static void focusCallback(GLFWwindow* window, int focused);

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void keyTypedCallback(GLFWwindow* window, unsigned int codepoint);

        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);

        static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    private:
        // needed to check if we need to init/terminate glfw
        static u64 m_GlfwWindowCount;
        GLFWwindow* m_Window = nullptr;

        std::mutex m_EventQueueMutex;

    };

}

#endif

