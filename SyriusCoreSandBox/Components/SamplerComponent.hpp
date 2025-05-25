#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class SamplerComponent: public IComponent {
public:
    SamplerComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~SamplerComponent() override = default;

    void onUpdate(Duration deltaTime) override;

    void onImGui(ImGuiWindowData &windowData) override;

private:
    ResourceView<Sampler> m_Sampler;
};

class SamplerFactory: public IComponentFactory {
public:
    SamplerFactory() = default;
    ~SamplerFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<SamplerComponent>(window, context, container);
    }

    std::string provides() const override {
        return "Sampler";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};