#pragma once

#include "../SyriusCoreDev/Layer.hpp"

class BasicLayer: public Layer{
public:
    BasicLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~BasicLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ResourceView<Texture2D> m_Texture;
    ResourceView<Sampler> m_Sampler;
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;


};