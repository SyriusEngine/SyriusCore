#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class CubeMapLayer: public IComponent {
public:
    CubeMapLayer(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~CubeMapLayer() override = default;

    void onUpdate(Duration deltaTime) override;

private:
    ResourceView<CubeMap> m_CubeMap;
};

class CubeMapFactory: public IComponentFactory {
public:
    CubeMapFactory() = default;
    ~CubeMapFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<CubeMapLayer>(window, context, container);
    }

    std::string provides() const override {
        return "CubeMap";
    }

    std::vector<std::string> requires() const override {
        return {
            "Projection",
            "Camera",
            "ShaderComponent",
            "Sampler",
            "MeshComponent",
            "DepthTest",
        };
    }
};