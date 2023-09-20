#pragma once

#include "SyriusCoreDev/Layer.hpp"

class AppLayer: public Layer{
public:
    AppLayer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~AppLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

    void onAttach() override;

    void onDetach() override;

private:

    void updateTime();

    void render();

    void renderImGui();

private:
    double m_DeltaTime;
    double m_LastFrameTime;

    ResourceView<Texture2D> m_Texture;
    ResourceView<Sampler> m_Sampler;
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;


};