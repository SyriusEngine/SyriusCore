#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class ShaderStorageBufferLayer: public IComponent {
public:
    ShaderStorageBufferLayer(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~ShaderStorageBufferLayer() override = default;

    void onUpdate(Duration deltaTime) override;

    void onImGui(ImGuiWindowData &windowData) override;

private:

    void imGuiDrawTransform();

    void imGuiDrawVertexArray();

    void imGuiDrawMeshSelector();

    void setTransformData();

    void setMesh(const MeshData& meshData);

private:
    std::vector<Transform> m_Transforms;
    ResourceView<ShaderStorageBuffer> m_TransformBuffer;

    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexArray> m_VertexArray;
    SR_DRAW_MODE m_CurrentDrawMode = SR_DRAW_TRIANGLES;
    Size m_VertexCount = 0;
    Size m_IndexCount = 0;
    Size m_InstanceCount = 0;

    glm::vec3 m_Translate = glm::vec3(0.0f);
    glm::vec3 m_Rotate = glm::vec3(0.0f);
    glm::vec3 m_Scale = glm::vec3(1.0f);
};

class ShaderStorageBufferFactory: public IComponentFactory {
public:
    ShaderStorageBufferFactory() = default;
    ~ShaderStorageBufferFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<ShaderStorageBufferLayer>(window, context, container);
    }

    std::string provides() const override {
        return "ShaderStorageBuffer";
    }

    std::vector<std::string> requires() const override {
        return {
            "Sampler",
            "Projection",
            "Camera",
            "ShaderComponent",
            "DepthTest",
            "Texture",
        };
    }
};
