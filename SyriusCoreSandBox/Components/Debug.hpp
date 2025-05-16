#pragma once

#include "../Core/IComponentFactory.hpp"

class Debug: public IComponent{
public:
    Debug(const UP<SyriusWindow>& window, const ResourceView<Context>& context);

    ~Debug() override = default;

    void onImGui(ImGuiWindowData& windowData) override;
};

class DebugComponentFactory: public IComponentFactory {
public:
    DebugComponentFactory() = default;
    ~DebugComponentFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context) override {
        return createSP<Debug>(window, context);
    }

    std::string provides() const override {
        return "Debug";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};
