#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class PBRLayer: public IComponent {
public:
    PBRLayer(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~PBRLayer() override = default;
};

class PBRFactory: public IComponentFactory {
public:
    PBRFactory() = default;
    ~PBRFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<PBRLayer>(window, context, container);
    }

    std::string provides() const override {
        return "PBR";
    }

    std::vector<std::string> requires() const override {
        return {
            "Projection",
            "Camera",
            "ShaderComponent",
            "DepthTest",
            "Sampler",
            "Material",
            "MeshComponent",
        };
    }
};