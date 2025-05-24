#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class FrameBufferLayer: public IComponent {
public:
    FrameBufferLayer(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~FrameBufferLayer() override = default;

    void onUpdate(Duration deltaTime) override;

    void onEvent(const Event &event) override;

    void onImGui(ImGuiWindowData &windowData) override;

private:

    void imGuiFrameBuffer(ImGuiWindowData &windowData);

private:
    UP<ComponentContainer> m_PrivContainer;

    ResourceView<VertexArray> m_ScreenVertexArray;
    ResourceView<FrameBuffer> m_FrameBuffer;
    ShaderProgram m_ScreenProgram;

    i32 m_SelectedTexture = 0;
    bool m_EnableDepthTest = true;
};

class FrameBufferLayerFactory: public IComponentFactory {
public:
    FrameBufferLayerFactory() = default;
    ~FrameBufferLayerFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<FrameBufferLayer>(window, context, container);
    }

    std::string provides() const override {
        return "FrameBuffer";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};