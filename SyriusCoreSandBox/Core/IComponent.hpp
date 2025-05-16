#pragma once

#include "../SandBoxInclude.hpp"

struct ImGuiWindowData {
    ImVec2 position = {0.0f, 0.0f};
    ImVec2 size = {200.0f, 0.0f};
};

class IComponent {
public:
    IComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context);

    virtual ~IComponent() = default;

    virtual void onUpdate(Duration deltaTime);

    virtual void onEvent(const Event& event);

    virtual void onImGui(ImGuiWindowData& windowData);

protected:
    void imGuiBegin(const ImGuiWindowData & windowData, std::string_view name);

    void imGuiEnd(ImGuiWindowData& windowData);

protected:

    const UP<SyriusWindow>& m_Window;
    const ResourceView<Context>& m_Context;

};
