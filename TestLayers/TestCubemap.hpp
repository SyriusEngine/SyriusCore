#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"
#include "../SyriusCoreDev/Camera.hpp"
#include "../SyriusCoreDev/Projection.hpp"

class TestCubemap: public Layer{
public:
    TestCubemap(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TestCubemap() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:

    void render();

    void renderImGui();

    void loadCubemap(const std::string& path);

private:
    ShaderLibrary m_TestShaderLibrary;

    ResourceView<VertexArray> m_VertexArray;
    ResourceView<Texture2D> m_Texture;
    ResourceView<Sampler> m_Sampler;
    ShaderProgram m_ShaderProgram;

    ResourceView<ConstantBuffer> m_LerpBuffer;
    ResourceView<ConstantBuffer> m_ModelDataBuffer;

    ResourceView<Cubemap> m_Cubemap;
    ShaderProgram m_CubemapProgram;
    ResourceView<VertexArray> m_CubemapVertexArray;

    Camera m_Camera;
    Projection m_Projection;

    double m_DeltaTime;
    double m_LastFrameTime;
    glm::vec3 m_Translation;
    float m_Lerp;

};