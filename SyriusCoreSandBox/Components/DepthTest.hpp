#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class DepthTest: public IComponent {
public:
    DepthTest(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~DepthTest() override = default;

    void onImGui(ImGuiWindowData& windowData) override;

private:
    bool m_EnableDepthTest = true;
};

class DepthTestFactory: public IComponentFactory {
public:
    DepthTestFactory() = default;
    ~DepthTestFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<DepthTest>(window, context, container);
    }

    std::string provides() const override {
        return "DepthTest";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};