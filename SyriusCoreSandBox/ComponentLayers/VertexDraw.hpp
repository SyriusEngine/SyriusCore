#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class VertexDraw: public IComponent {
public:
    VertexDraw(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~VertexDraw() override = default;

    void onUpdate(Duration deltaTime) override;

    void onImGui(ImGuiWindowData& windowData) override;

private:

    void imGuiVertexBufferPanel(ImGuiWindowData &windowData);

    void imGuiVertexArrayPanel(ImGuiWindowData &windowData);

private:
    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexBuffer> m_VertexBuffer;
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;
};

class VertexDrawFactory: public IComponentFactory {
public:
    VertexDrawFactory() = default;
    ~VertexDrawFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<VertexDraw>(window, context, container);
    }

    std::string provides() const override {
        return "VertexDraw";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};
