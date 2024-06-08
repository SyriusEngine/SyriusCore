#pragma once

#include "../TestEnvironment.hpp"
#include "../TestUtils.hpp"

struct ShaderStorage{
    ResourceView<Shader> shader;
    ResourceView<ShaderModule> vertexShader;
    ResourceView<ShaderModule> fragmentShader;
};

class FrameBufferTest: public testing::Test{
protected:
    void SetUp() override;

    void TearDown() override;

    static ResourceView<FrameBuffer> createFrameBuffer1CA();

    static ResourceView<FrameBuffer> createFrameBuffer2CA();

    static ResourceView<VertexArray> createScreenQuad(ShaderStorage& ss);

    static ShaderStorage createScreenShader1CA();

    static ShaderStorage createScreenShader2CA();
};