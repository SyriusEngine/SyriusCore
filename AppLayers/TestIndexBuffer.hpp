#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"

class TestIndexBuffer: public Layer{
public:
    TestIndexBuffer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TestIndexBuffer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ShaderLibrary m_TestShaderLibrary;

    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;
};