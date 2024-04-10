#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"
#include "../SyriusCoreDev/Camera.hpp"
#include "../SyriusCoreDev/Projection.hpp"

class TestFrameBuffer: public Layer{
public:
    TestFrameBuffer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TestFrameBuffer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:

    void render();

    void renderImGui();

private:
    ShaderLibrary m_TestShaderLibrary;

    ResourceView<VertexArray> m_CubeVertexArray;
    ResourceView<Texture2D> m_AwesomeFaceTexture;
    ResourceView<Sampler> m_Sampler;
    ShaderProgram m_GeometryProgram;

    ResourceView<ConstantBuffer> m_LerpBuffer;
    ResourceView<ConstantBuffer> m_ModelDataBuffer;

    ResourceView<FrameBuffer> m_FrameBuffer;
    ResourceView<VertexArray> m_ScreenVertexArray;
    ShaderProgram m_ScreenProgram;

    Camera m_Camera;
    Projection m_Projection;

    double m_DeltaTime;
    double m_LastFrameTime;
    glm::vec3 m_Translation;
    float m_Lerp;
    float m_TextureResample;

    // depth test params
    bool m_DepthTest;
    bool m_DepthMask;

    // frame buffer params
    bool m_SampleFromColorAttachment;


};