#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class MVP: public IComponent {
public:
    MVP(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~MVP() override = default;
};

class MVPFactory: public IComponentFactory {
public:
    MVPFactory() = default;
    ~MVPFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<MVP>(window, context, container);
    }

    std::string provides() const override {
        return "MVP";
    }

    std::vector<std::string> requires() const override {
        return {
            "Projection",
            "Camera",
            "ShaderComponent",
            "DepthTest",
            "MeshComponent",
        };
    }
};

