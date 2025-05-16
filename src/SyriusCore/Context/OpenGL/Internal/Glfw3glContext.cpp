#include "Glfw3glContext.hpp"

#include "../../../Dependencies/imgui/imgui_impl_glfw.h"
#include "../../../Dependencies/imgui/imgui_impl_opengl3.h"
#include "SyriusCore/Dependencies/imgui/implot.h"

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
        if (m_ImGuiContextCreated) {
            SR_LOG_WARNING("Glfw3glContext", "ImGui context was created but not destroyed! Destroying...");
            Glfw3glContext::terminateImGui();
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

    void Glfw3glContext::initImGui(const ImGuiDesc &desc) {
        Context::initImGui(desc);

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 150");
    }

    void Glfw3glContext::terminateImGui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        Context::terminateImGui();
    }

    void Glfw3glContext::onImGuiBegin() {
        SR_PRECONDITION(m_ImGuiContextCreated == true, "There does not exists an ImGui context");
        SR_PRECONDITION(m_IsImGuiRendering == false, "ImGuiRendering already started!")

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        m_IsImGuiRendering = true;

        SR_POSTCONDITION(m_IsImGuiRendering == true, "Failed to start ImGuiRendering");
    }

    void Glfw3glContext::onImGuiEnd() {
        SR_PRECONDITION(m_IsImGuiRendering == true, "onImGuiEnd() called before onImGuiBegin()")

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            makeCurrent();
        }
        m_IsImGuiRendering = false;
    }

}

#endif