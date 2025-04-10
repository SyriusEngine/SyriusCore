#include "Glfw3glContext.hpp"

#include "../../../Dependencies/imgui/imgui_impl_glfw.h"
#include "../../../Dependencies/imgui/imgui_impl_opengl3.h"

#if !defined(SR_PLATFORM_WIN64)

namespace Syrius {

    Glfw3glContext::Glfw3glContext(GLFWwindow *window, const ContextDesc &desc):
    GlContext(desc),
    m_Window(window){
        Glfw3glContext::makeCurrent();
        initGl();

        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);

        GlContext::createDefaultFrameBuffer(width, height, desc);
    }

    Glfw3glContext::~Glfw3glContext() {
        if (m_ImGuiContext){
            Glfw3glContext::destroyImGuiContext();
        }
        terminateGl();

    }

    void Glfw3glContext::setGlfw3glContextHints(const ContextDesc &desc) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    void Glfw3glContext::createImGuiContext() {
        SR_PRECONDITION(!m_ImGuiContext, "There exists already an ImGui context")

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 150");

        m_ImGuiContext = ImGui::GetCurrentContext();

        SR_POSTCONDITION(m_ImGuiContext, "Failed to create ImGui context");
    }

    void Glfw3glContext::destroyImGuiContext() {
        SR_PRECONDITION(m_ImGuiContext, "There does not exists an ImGui context");

        ImGui::SetCurrentContext(m_ImGuiContext);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        m_ImGuiContext = nullptr;
    }

    void Glfw3glContext::onImGuiBegin() {
        SR_PRECONDITION(m_ImGuiContext, "There does not exists an ImGui context");

        ImGui::SetCurrentContext(m_ImGuiContext);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Glfw3glContext::onImGuiEnd() {
        SR_PRECONDITION(m_ImGuiContext, "There does not exists an ImGui context");

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}

#endif