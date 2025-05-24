#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class MVPTextured: public IComponent {
public:
    MVPTextured(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~MVPTextured() override = default;
};

class MVPTexturedFactory: public IComponentFactory {
public:
    MVPTexturedFactory() = default;
    ~MVPTexturedFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<MVPTextured>(window, context, container);
    }

    std::string provides() const override {
        return "MVPTextured";
    }

    std::vector<std::string> requires() const override {
        return {
            "Projection",
            "Camera",
            "ShaderComponent",
            "DepthTest",
            "Sampler",
            "Texture",
            "MeshComponent",
        };
    }
};