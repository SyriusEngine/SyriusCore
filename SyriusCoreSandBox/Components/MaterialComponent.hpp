#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

struct MaterialHandle {
    ResourceView<Texture2D> albedo;
    ResourceView<Texture2D> normal;
    ResourceView<Texture2D> metallic;
    ResourceView<Texture2D> roughness;
    ResourceView<Texture2D> ao;
};

class MaterialComponent: public IComponent {
public:
    MaterialComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~MaterialComponent() override = default;

    void onUpdate(Duration deltaTime) override;

    void onImGui(ImGuiWindowData &windowData) override;

private:

    void loadMaterial(i32 id, const std::string& name);

    ResourceView<Texture2D> loadTextureFromFile(const fs::path& path) const;

private:
    i32 m_SelectedMaterial = 0;
    std::unordered_map<i32, MaterialHandle> m_Handles;

    TextureParameters m_Parameters;
    ResourceView<ConstantBuffer> m_ParametersBuffer;
};

class MaterialFactory: public IComponentFactory {
public:
    MaterialFactory() = default;
    ~MaterialFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<MaterialComponent>(window, context, container);
    }

    std::string provides() const override {
        return "Material";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};