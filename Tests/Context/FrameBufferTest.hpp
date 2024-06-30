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

    template<typename T>
    bool isPixelCorrect(Resource<Image>& img, T r, T g, T b, T a){
        auto data = reinterpret_cast<T*>(img->getData());
        auto width = img->getWidth();
        auto height = img->getHeight();
        for (uint32 i = 0; i < width * height * 4; i+=4){
            if (data[i] != r || data[i+1] != g || data[i+2] != b || data[i+3] != a){
                return false;
            }
        }
        return true;
    }
};