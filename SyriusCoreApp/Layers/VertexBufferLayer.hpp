#pragma once

#include "../Layer.hpp"

class VertexBufferLayer: public Layer{
public:
    VertexBufferLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~VertexBufferLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexBuffer> m_VertexBuffer;
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;
};