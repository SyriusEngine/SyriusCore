#pragma once

#include "../Layer.hpp"
#include "../Camera.hpp"
#include "../Projection.hpp"
#include <chrono>

class DepthTestLayer: public Layer{
public:
    DepthTestLayer(ResourceView<Context>& context, const UP<SyriusWindow>& window, EasyIni::Configuration& config);

    ~DepthTestLayer() override;

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