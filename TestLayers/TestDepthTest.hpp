#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"
#include "../SyriusCoreDev/Camera.hpp"
#include "../SyriusCoreDev/Projection.hpp"

class TestDepthTest: public Layer{
public:
    TestDepthTest(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TestDepthTest() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:

    void render();

    void renderImGui();

private:
    ShaderLibrary m_TestShaderLibrary;

    ResourceView<VertexArray> m_VertexArray;
    ResourceView<Texture2D> m_Texture;
    ResourceView<Sampler> m_Sampler;
    ShaderProgram m_ShaderProgram;

    ResourceView<ConstantBuffer> m_LerpBuffer;
    ResourceView<ConstantBuffer> m_ModelDataBuffer;

    Camera m_Camera;
    Projection m_Projection;

    double m_DeltaTime;
    double m_LastFrameTime;
    glm::vec3 m_Translation;
    float m_Lerp;

    // depth test params
    bool m_DepthTest;

};