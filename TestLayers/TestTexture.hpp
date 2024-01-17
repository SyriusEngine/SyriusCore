#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"

class TestTexture: public Layer{
public:
    TestTexture(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TestTexture() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:

    void onRender();

    void onImGuiRender();

private:
    ShaderLibrary m_TestShaderLibrary;

    ResourceView<VertexArray> m_VertexArray;
    ResourceView<Texture2D> m_Texture;
    ResourceView<Sampler> m_Sampler;
    ShaderProgram m_ShaderProgram;

    bool m_UseHdrTexture;
};