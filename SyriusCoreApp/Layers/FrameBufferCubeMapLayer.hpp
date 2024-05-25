#pragma once

#include "../Layer.hpp"
#include "../Camera.hpp"
#include "../Projection.hpp"

class FrameBufferCubeMapLayer: public Layer{
public:
    FrameBufferCubeMapLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~FrameBufferCubeMapLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;

    ResourceView<ConstantBuffer> m_TransformBuffer;
    ResourceView<Sampler> m_Sampler;
    ResourceView<Texture2D> m_Texture1;

    Camera m_Camera;
    Projection m_Projection;
};
