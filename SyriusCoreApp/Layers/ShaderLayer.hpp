#pragma once

#include "../Layer.hpp"

struct Parameters{
    glm::vec2 resolution = glm::vec2(0.0f, 0.0f);
    float time = 0.0f;
    float deltaTime = 0.0f;
    glm::vec4 mouse = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
};

class ShaderLayer: public Layer{
public:
    ShaderLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~ShaderLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:

    void shaderSelectorPanel();

private:
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;

    ResourceView<ConstantBuffer> m_ParameterBuffer;
    ResourceView<Texture2D> m_Texture1;
    ResourceView<Sampler> m_Sampler;

    Parameters m_Params;
    double m_StartTime;

    bool m_UseTime = true;
    float t = 0.0;
};