#pragma once

#include "../Layer.hpp"
#include "../Camera.hpp"
#include "../Projection.hpp"
#include <chrono>

struct Light{
    glm::vec4 position = { 0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 ambient = { 0.2f, 0.2f, 0.2f, 1.0f};
    glm::vec4 diffuse = { 0.5f, 0.5f, 0.5f, 1.0f};
    glm::vec4 specular = { 1.0f, 1.0f, 1.0f, 1.0f};
};

struct Material{
    glm::vec4 ambient = { 1.0f, 0.5f, 0.31f, 1.0f};
    glm::vec4 diffuse = { 1.0f, 0.5f, 0.31f, 1.0f};
    glm::vec4 specular = { 0.5f, 0.5f, 0.5f, 1.0f};
    float shininess = 32.0f;
    float padding[3];
};

struct LightData{
    Light lights[4];
    uint32 lightCount = 0;
    float padding[3];
};

class PhongLightLayer: public Layer{
public:
    PhongLightLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~PhongLightLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;

    ResourceView<ConstantBuffer> m_TransformBuffer;
    ResourceView<ConstantBuffer> m_TextureParametersBuffer;
    ResourceView<Texture2D> m_Texture1;
    ResourceView<Texture2D> m_Texture2;
    ResourceView<Sampler> m_Sampler;

    Camera m_Camera;
    Projection m_Projection;
};