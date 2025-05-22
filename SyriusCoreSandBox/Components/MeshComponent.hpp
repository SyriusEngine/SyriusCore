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
};
