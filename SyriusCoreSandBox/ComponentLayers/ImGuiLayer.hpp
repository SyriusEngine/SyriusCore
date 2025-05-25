#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class ImGuiLayer: public IComponent {
public:
    ImGuiLayer(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~ImGuiLayer() override = default;

    void onImGui(ImGuiWindowData &windowData) override;
};

class ImGuiFactory: public IComponentFactory {
public:
    ImGuiFactory() = default;
    ~ImGuiFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<ImGuiLayer>(window, context, container);
    }

    std::string provides() const override {
        return "ImGui";
    }

    std::vector<std::string> requires() const override{
        return {};
    }
};