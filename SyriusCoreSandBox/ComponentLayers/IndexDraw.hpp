#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class IndexDraw: public IComponent {
public:
    IndexDraw(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~IndexDraw() override = default;

    void onUpdate(Duration deltaTime) override;

    void onImGui(ImGuiWindowData &windowData) override;

private:

    void imGuiVertexBufferPanel(ImGuiWindowData &windowData);

    void imGuiIndexBufferPanel(ImGuiWindowData &windowData);

    void imGuiVertexArrayPanel(ImGuiWindowData &windowData);

private:
    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexBuffer> m_VertexBuffer;
    ResourceView<IndexBuffer> m_IndexBuffer;
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;

    float m_Size = 0.5f;
};

class IndexDrawFactory: public IComponentFactory {
public:
    IndexDrawFactory() = default;
    ~IndexDrawFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<IndexDraw>(window, context, container);
    }

    std::string provides() const override {
        return "IndexDraw";
    }

    std::vector<std::string> requires() const override{
        return {};
    }
};