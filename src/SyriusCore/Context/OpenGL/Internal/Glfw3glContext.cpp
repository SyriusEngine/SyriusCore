#include "Glfw3glContext.hpp"

#if !defined(SR_PLATFORM_WIN64)

namespace Syrius {

    Glfw3glContext::Glfw3glContext(GLFWwindow *window, const ContextDesc &desc):
    GlContext(desc),
    m_Window(window){
        initGl(desc);
    }

    Glfw3glContext::~Glfw3glContext() {
        if (m_ImGuiContext){
            destroyImGuiContext();
        }
        terminateGl();

    }

    void Glfw3glContext::setGlfw3glContextHints(const ContextDesc &desc) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_RED_BITS, desc.redBits);
        glfwWindowHint(GLFW_GREEN_BITS, desc.greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, desc.blueBits);
        glfwWindowHint(GLFW_ALPHA_BITS, desc.alphaBits);
        glfwWindowHint(GLFW_DEPTH_BITS, desc.depthBits);
        glfwWindowHint(GLFW_STENCIL_BITS, desc.stencilBits);
#if defined(SR_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    }

    void Glfw3glContext::makeCurrent() {
        glfwMakeContextCurrent(m_Window);
    }

    void Glfw3glContext::swapBuffers() {
        glfwSwapBuffers(m_Window);
    }

    void Glfw3glContext::setVerticalSynchronisation(const bool enable) {
        glfwSwapInterval(enable ? 1 : 0);
    }

    FramebufferSize Glfw3glContext::getFramebufferSize() {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(m_Window, &width, &height);
        FramebufferSize size;
        size.m_Width = width;
        size.m_Height = height;
        return size;
    }

    void Glfw3glContext::createImGuiContext() {

    }

    void Glfw3glContext::destroyImGuiContext() {

    }

    void Glfw3glContext::onImGuiBegin() {

    }

    void Glfw3glContext::onImGuiEnd() {

    }

}

#endif