#pragma once

#include "../Layer.hpp"
#include "../Camera.hpp"
#include "../Projection.hpp"
#include <chrono>

class CubeMapLayer: public Layer{
public:
    CubeMapLayer(ResourceView<Context>& context, const UP<SyriusWindow>& window, EasyIni::Configuration& config);

    ~CubeMapLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;

    ResourceView<ConstantBuffer> m_TransformBuffer;
    ResourceView<Sampler> m_Sampler;
    ResourceView<CubeMap> m_CubeMap;

    Camera m_Camera;
    ProjectionComponent m_Projection;
};