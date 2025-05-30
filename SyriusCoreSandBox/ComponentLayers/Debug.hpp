#pragma once

#include "../Core/IComponentFactory.hpp"

class Debug: public IComponent{
public:
    Debug(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~Debug() override = default;

    void onImGui(ImGuiWindowData& windowData) override;
};

class DebugFactory: public IComponentFactory {
public:
    DebugFactory() = default;
    ~DebugFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<Debug>(window, context, container);
    }

    std::string provides() const override {
        return "Debug";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};
