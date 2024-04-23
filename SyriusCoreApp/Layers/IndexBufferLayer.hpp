#pragma once

#include "../Layer.hpp"

class IndexBufferLayer: public Layer{
public:
    IndexBufferLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~IndexBufferLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexBuffer> m_VertexBuffer;
    ResourceView<IndexBuffer> m_IndexBuffer;
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;
};