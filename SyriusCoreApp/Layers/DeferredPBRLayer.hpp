#pragma once

#include "../Layer.hpp"
#include "../Camera.hpp"
#include "../Material.hpp"
#include "../Projection.hpp"

struct LightData{
    glm::vec4 positions[300];
    glm::vec4 colors[300];
    glm::uvec4 count;
};

class DeferredPBRLayer: public Layer{
public:
    DeferredPBRLayer(ResourceView<Context>& context, const UP<SyriusWindow>& window, EasyIni::Configuration& config);

    ~DeferredPBRLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:

    void imGuiGBufferPanel();

private:
    Camera m_Camera;
    ProjectionComponent m_Projection;
    ResourceView<Sampler> m_Sampler;

    ResourceView<ConstantBuffer> m_TransformBuffer;
    ResourceView<ConstantBuffer> m_TextureParametersBuffer;
    ResourceView<VertexArray> m_Model;
    Material m_Material;
    ShaderProgram m_GeometryPass;

    ResourceView<FrameBuffer> m_GBuffer;

    ResourceView<ConstantBuffer> m_LightBuffer;
    ResourceView<VertexArray> m_ScreenRectangle;
    ShaderProgram m_LightPass;
    LightData m_LightData;

};