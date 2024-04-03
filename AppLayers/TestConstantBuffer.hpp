#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"

class TestConstantBuffer: public Layer{
public:
    TestConstantBuffer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TestConstantBuffer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ShaderLibrary m_TestShaderLibrary;

    ResourceView<VertexArray> m_VertexArray;
    ResourceView<ConstantBuffer> m_TransformBuffer;
    ResourceView<ConstantBuffer> m_LerpBuffer;
    ResourceView<Texture2D> m_Texture;
    ResourceView<Sampler> m_Sampler;
    ShaderProgram m_ShaderProgram;

    glm::vec3 m_Translation;
    float m_Lerp;
};