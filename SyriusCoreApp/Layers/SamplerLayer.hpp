#pragma once

#include "../Layer.hpp"

class SamplerLayer: public Layer{
public:
    SamplerLayer(ResourceView<Context>& context, const UP<SyriusWindow>& window, EasyIni::Configuration& config);

    ~SamplerLayer() override;

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

};