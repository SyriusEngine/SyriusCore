#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

struct LightData{
    glm::vec4 positions[300];
    glm::vec4 colors[300];
    glm::uvec4 count;
};


class DeferredPBRLayer: public IComponent {
public:
    DeferredPBRLayer(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~DeferredPBRLayer() override = default;

    void onUpdate(Duration deltaTime) override;

    void onEvent(const Event &event) override;

    void onImGui(ImGuiWindowData &windowData) override;

private:

    void imGuiFrameBuffer(ImGuiWindowData &windowData);

private:
    UP<ComponentContainer> m_PrivContainer;

    LightData m_LightData;
    ResourceView<ConstantBuffer> m_LightBuffer;
    ResourceView<VertexArray> m_ScreenVertexArray;
    ResourceView<FrameBuffer> m_GBuffer;
    ShaderProgram m_ScreenProgram;

    bool m_EnableDepthTest = true;
};

class DeferredPBRFactory: public IComponentFactory {
public:
    DeferredPBRFactory() = default;
    ~DeferredPBRFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<DeferredPBRLayer>(window, context, container);
    }

    std::string provides() const override {
        return "DeferredPBR";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};