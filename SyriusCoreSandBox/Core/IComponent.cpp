#include "IComponent.hpp"

IComponent::IComponent(const UP<SyriusWindow> &window, const ResourceView<Context> &context):
m_Window(window), m_Context(context){

}

void IComponent::onUpdate(Duration deltaTime) {  }

void IComponent::onEvent(const Event &event) {  }

void IComponent::onImGui(ImGuiWindowData &windowData) {  }

void IComponent::imGuiBegin(const ImGuiWindowData &windowData, const std::string_view name) {
    ImGui::SetNextWindowSize(windowData.size);
    ImGui::SetNextWindowPos(windowData.position);
    ImGui::Begin(name.data(), nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize);
}

void IComponent::imGuiEnd(ImGuiWindowData &windowData) {
    ImVec2 lastSize = ImGui::GetWindowSize();
    ImGui::End();
    if (windowData.size.y + lastSize.y > m_Window->getHeight()){
        windowData.position.y = 0.0f;
        windowData.position.x = m_Window->getWidth() - lastSize.x;
    }
    else{
        windowData.position.y += lastSize.y;
    }
}




