#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"

class TestVertexBuffer: public Layer{
public:
    TestVertexBuffer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TestVertexBuffer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:
    ShaderLibrary m_CShaderLibrary;

    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexBuffer> m_VertexBuffer;
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;
};