#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"

class TestShaderStorageBuffer: public Layer {
public:
    TestShaderStorageBuffer(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config);

    ~TestShaderStorageBuffer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

private:

    void render();

    void renderImGui();

private:
    ShaderLibrary m_TestShaderLibrary;

    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexBuffer> m_VertexBuffer;
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;

    ResourceView<ShaderStorageBuffer> m_SSBO;

    bool m_UsePositionAsColor;

};