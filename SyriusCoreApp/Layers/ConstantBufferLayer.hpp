#pragma once

#include "../Layer.hpp"

struct ColorData{
    glm::vec4 color = glm::vec4(0.2f, 0.5f, 0.8f, 1.0f);
    glm::vec4 lerpFactor = glm::vec4(0.5f);
};

class ConstantBufferLayer: public Layer{
public:
    ConstantBufferLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~ConstantBufferLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:

    void imGuiRenderColorDataConstantBuffer(ResourceView<ConstantBuffer>& );

private:
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;

    ResourceView<ConstantBuffer> m_TransformBuffer;
    ResourceView<ConstantBuffer> m_ColorDataBuffer;

    ColorData m_ColorData;

};