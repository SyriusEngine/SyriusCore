#pragma once

#include "../Layer.hpp"
#include "../Camera.hpp"
#include "../Projection.hpp"
#include <chrono>


class FrameBufferLayer: public Layer{
public:
    FrameBufferLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~FrameBufferLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;

    ResourceView<VertexArray> m_ScreenVertexArray;
    ShaderProgram m_ScreenProgram;

    ResourceView<FrameBuffer> m_FrameBuffer;

    ResourceView<ConstantBuffer> m_TransformBuffer;
    ResourceView<ConstantBuffer> m_TextureParametersBuffer;
    ResourceView<Texture2D> m_Texture1;
    ResourceView<Texture2D> m_Texture2;

    ResourceView<Sampler> m_Sampler;

    Camera m_Camera;
    Projection m_Projection;

    double m_DeltaTime;
    double m_LastFrameTime;
    int32 m_SelectedTexture;
};