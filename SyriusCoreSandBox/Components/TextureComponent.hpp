#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class TextureComponent: public IComponent {
public:
    TextureComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~TextureComponent() override = default;

    void onUpdate(Duration deltaTime) override;

    void onImGui(ImGuiWindowData &windowData) override;

private:
    TextureParameters m_Parameters;
    ResourceView<ConstantBuffer> m_ParametersBuffer;
    ResourceView<Texture2D> m_AwesomeFace;
    ResourceView<Texture2D> m_Instagram;
};

class TextureFactory: public IComponentFactory {
public:
    TextureFactory() = default;
    ~TextureFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<TextureComponent>(window, context, container);
    }

    std::string provides() const override {
        return "Texture";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};