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

    void imGuiDrawTransform();

    void imGuiDrawVertexArray();

    void imGuiDrawMeshSelector();

    void setTransformData() const;

    void setMesh(const MeshData& meshData);

private:
    Transform m_Transform;
    ResourceView<ConstantBuffer> m_TransformBuffer;

    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexArray> m_VertexArray;
    SR_DRAW_MODE m_CurrentDrawMode = SR_DRAW_TRIANGLES;
    Size m_VertexCount = 0;
    Size m_IndexCount = 0;

    glm::vec3 m_Translate = glm::vec3(0.0f);
    glm::vec3 m_Rotate = glm::vec3(0.0f);
    glm::vec3 m_Scale = glm::vec3(1.0f);
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
