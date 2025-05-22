#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

struct Transform {
    glm::mat4 model = glm::mat4(1.0f);
};

class MeshComponent: public IComponent {
public:
    MeshComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~MeshComponent() override = default;

    void onUpdate(Duration deltaTime) override;

    void onImGui(ImGuiWindowData &windowData) override;

private:
    Transform m_Transform;
    ResourceView<ConstantBuffer> m_TransformBuffer;

    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexBuffer> m_VertexBuffer;
    ResourceView<IndexBuffer> m_IndexBuffer;
    ResourceView<VertexArray> m_VertexArray;
};

class MeshComponentFactory: public IComponentFactory {
public:
    MeshComponentFactory() = default;
    ~MeshComponentFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<MeshComponent>(window, context, container);
    }

    std::string provides() const override {
        return "MeshComponent";
    }

    std::vector<std::string> requires() const override{
        return {
            "ShaderComponent"
        };
    }
};
