#pragma once

#include "../Layer.hpp"

class TextureLayer: public Layer{
public:
    TextureLayer(ResourceView<Context>& context, const UP<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TextureLayer() override;

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