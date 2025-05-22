#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

struct ProjectionData{
    glm::mat4 projection;
};

class Projection: public IComponent {
public:
    Projection(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~Projection() override = default;

    void onUpdate(Duration deltaTime) override;

    void onEvent(const Event &event) override;

    void onImGui(ImGuiWindowData& windowData) override;

private:

    void setProjection();

private:
    float m_Fov = 45.0f;
    float m_Near = 0.1f;
    float m_Far = 100.0f;

    ProjectionData m_ProjectionData;
    ResourceView<ConstantBuffer> m_Buffer;
};

class ProjectionFactory: public IComponentFactory {
public:
    ProjectionFactory() = default;
    ~ProjectionFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<Projection>(window, context, container);
    }

    std::string provides() const override {
        return "Projection";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};