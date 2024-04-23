#pragma once

#include "../Layer.hpp"

struct TextureParameters{
    float lerpFactor = 0.5f;
    float scale = 1.0f;
    float padding[2] = {0.0f, 0.0f};
};

class SamplerLayer: public Layer{
public:
    SamplerLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~SamplerLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:

    void imGuiRenderTextureParameters(ResourceView<ConstantBuffer>& constantBuffer);

private:
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;

    ResourceView<ConstantBuffer> m_TransformBuffer;
    ResourceView<ConstantBuffer> m_TextureParametersBuffer;
    ResourceView<Texture2D> m_Texture1;
    ResourceView<Texture2D> m_Texture2;
    ResourceView<Sampler> m_Sampler;

    TextureParameters m_TextureParameters;

};