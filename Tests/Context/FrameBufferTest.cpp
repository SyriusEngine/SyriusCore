#include "FrameBufferTest.hpp"
#include "FrameBufferShaders.hpp"

void FrameBufferTest::SetUp() {
    Test::SetUp();
}

void FrameBufferTest::TearDown() {
    Test::TearDown();
}

constexpr uint32 s_Width = 50;
constexpr uint32 s_Height = 50;

ResourceView<FrameBuffer> FrameBufferTest::createFrameBuffer1CA() {
    auto fbLayout = TestEnvironment::m_Context->createFrameBufferLayout();
    ViewportDesc vpDesc;
    vpDesc.width = s_Width;
    vpDesc.height = s_Height;
    fbLayout->addViewportDesc(vpDesc);
    ColorAttachmentDesc colorDesc;
    colorDesc.format = SR_TEXTURE_RGBA_F32;
    colorDesc.width = s_Width;
    colorDesc.height = s_Height;
    colorDesc.clearColor[0] = 1.0f;
    colorDesc.clearColor[1] = 0.0f;
    colorDesc.clearColor[2] = 0.0f;
    colorDesc.clearColor[3] = 1.0f;
    fbLayout->addColorAttachmentDesc(colorDesc);
    return TestEnvironment::m_Context->createFrameBuffer(fbLayout);
}

ResourceView<FrameBuffer> FrameBufferTest::createFrameBuffer2CA() {
    auto fbLayout = TestEnvironment::m_Context->createFrameBufferLayout();
    ViewportDesc vpDesc;
    vpDesc.width = s_Width;
    vpDesc.height = s_Height;
    fbLayout->addViewportDesc(vpDesc);
    ColorAttachmentDesc ca1Desc;
    ca1Desc.format = SR_TEXTURE_RGBA_F32;
    ca1Desc.width = s_Width;
    ca1Desc.height = s_Height;
    ca1Desc.clearColor[0] = 1.0f;
    ca1Desc.clearColor[1] = 0.0f;
    ca1Desc.clearColor[2] = 0.0f;
    ca1Desc.clearColor[3] = 1.0f;
    fbLayout->addColorAttachmentDesc(ca1Desc);

    ColorAttachmentDesc ca2Desc;
    ca2Desc.format = SR_TEXTURE_RGBA_UI8;
    ca2Desc.width = s_Width;
    ca2Desc.height = s_Height;
    ca2Desc.clearColor[0] = 0.0f;
    ca2Desc.clearColor[1] = 1.0f;
    ca2Desc.clearColor[2] = 0.0f;
    ca2Desc.clearColor[3] = 1.0f;
    fbLayout->addColorAttachmentDesc(ca2Desc);

    return TestEnvironment::m_Context->createFrameBuffer(fbLayout);
}

ResourceView<VertexArray> FrameBufferTest::createScreenQuad(ShaderStorage& ss) {
    auto vLayout = TestEnvironment::m_Context->createVertexLayout();
    vLayout->addAttribute("Position", SR_FLOAT32_3);
    vLayout->addAttribute("Color", SR_FLOAT32_3);

    VertexBufferDesc vbDesc;
    vbDesc.count = 4;
    vbDesc.data = s_ScreenVertices.data();
    vbDesc.layout = vLayout;
    auto vb = TestEnvironment::m_Context->createVertexBuffer(vbDesc);

    IndexBufferDesc ibDesc;
    ibDesc.count = 6;
    ibDesc.data = s_RectangleIndices.data();
    ibDesc.dataType = SR_UINT32;
    auto ib = TestEnvironment::m_Context->createIndexBuffer(ibDesc);

    VertexArrayDesc vaDesc;
    vaDesc.vertexBuffer = vb;
    vaDesc.indexBuffer = ib;
    vaDesc.vertexShader = ss.vertexShader;
    return TestEnvironment::m_Context->createVertexArray(vaDesc);
}

ShaderStorage FrameBufferTest::createScreenShader1CA() {
    ShaderStorage ss;
    ShaderModuleDesc vsmDesc;
    vsmDesc.shaderType = SR_SHADER_VERTEX;

    ShaderModuleDesc fsmDesc;
    fsmDesc.shaderType = SR_SHADER_FRAGMENT;

    if (TestEnvironment::m_API == SR_API_OPENGL){
        vsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
        fsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
        vsmDesc.code = s_GLSLVertexShader;
        fsmDesc.code = s_GLSLFragmentShader1CA;
    }
    else if (TestEnvironment::m_API == SR_API_D3D11){
        vsmDesc.language = SR_SHADER_LANGUAGE_HLSL;
        fsmDesc.language = SR_SHADER_LANGUAGE_HLSL;
        vsmDesc.code = s_HLSLVertexShader;
        fsmDesc.code = s_HLSLFragmentShader1CA;
    }
    ss.vertexShader = TestEnvironment::m_Context->createShaderModule(vsmDesc);
    ss.fragmentShader = TestEnvironment::m_Context->createShaderModule(fsmDesc);

    ShaderDesc sDesc;
    sDesc.vertexShader = ss.vertexShader;
    sDesc.fragmentShader = ss.fragmentShader;

    ss.shader =  TestEnvironment::m_Context->createShader(sDesc);
    return ss;
}

ShaderStorage FrameBufferTest::createScreenShader2CA() {
    ShaderStorage ss;
    ShaderModuleDesc vsmDesc;
    vsmDesc.shaderType = SR_SHADER_VERTEX;

    ShaderModuleDesc fsmDesc;
    fsmDesc.shaderType = SR_SHADER_FRAGMENT;

    if (TestEnvironment::m_API == SR_API_OPENGL){
        vsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
        fsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
        vsmDesc.code = s_GLSLVertexShader;
        fsmDesc.code = s_GLSLFragmentShader2CA;
    }
    else if (TestEnvironment::m_API == SR_API_D3D11){
        vsmDesc.language = SR_SHADER_LANGUAGE_HLSL;
        fsmDesc.language = SR_SHADER_LANGUAGE_HLSL;
        vsmDesc.code = s_HLSLVertexShader;
        fsmDesc.code = s_HLSLFragmentShader2CA;
    }
    ss.vertexShader = TestEnvironment::m_Context->createShaderModule(vsmDesc);
    ss.fragmentShader = TestEnvironment::m_Context->createShaderModule(fsmDesc);

    ShaderDesc sDesc;
    sDesc.vertexShader = ss.vertexShader;
    sDesc.fragmentShader = ss.fragmentShader;

    ss.shader =  TestEnvironment::m_Context->createShader(sDesc);
    return ss;
}


TEST_F(FrameBufferTest, CreateFrameBufferNoAttachments){
    auto fbLayout = TestEnvironment::m_Context->createFrameBufferLayout();
    bool failed = false;
    try{
        auto fb = TestEnvironment::m_Context->createFrameBuffer(fbLayout);
    }catch(const std::runtime_error& e){
        failed = true;
    }
    EXPECT_TRUE(failed);
}

TEST_F(FrameBufferTest, ClearFrameBuffer1ColorAttachment){
    auto fb = createFrameBuffer1CA();

    fb->clear();

    auto ca1 = fb->getColorAttachment(0);
    auto img = ca1->getData();

    EXPECT_EQ(img->getWidth(), s_Width);
    EXPECT_EQ(img->getHeight(), s_Height);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_F32);

    EXPECT_TRUE(isPixelCorrect<float>(img, 1.0f, 0.0f, 0, 1.0f));
}

TEST_F(FrameBufferTest, ClearFrameBuffer2ColorAttachments){
    auto fb = createFrameBuffer2CA();

    fb->clear();

    auto ca1 = fb->getColorAttachment(0);
    auto img1 = ca1->getData();

    auto ca2 = fb->getColorAttachment(1);
    auto img2 = ca2->getData();

    EXPECT_EQ(img1->getWidth(), s_Width);
    EXPECT_EQ(img1->getHeight(), s_Height);
    EXPECT_EQ(img1->getFormat(), SR_TEXTURE_RGBA_F32);

    EXPECT_EQ(img2->getWidth(), s_Width);
    EXPECT_EQ(img2->getHeight(), s_Height);
    EXPECT_EQ(img2->getFormat(), SR_TEXTURE_RGBA_UI8);

    EXPECT_TRUE(isPixelCorrect<float>(img1, 1.0f, 0.0f, 0.0f, 1.0f));
    EXPECT_TRUE(isPixelCorrect<uint8>(img2, 0, 255, 0, 255));
}

TEST_F(FrameBufferTest, GetColorAttachmentOutOfBounds){
    auto fb = createFrameBuffer1CA();

    EXPECT_DEATH(auto val = fb->getColorAttachment(1), "");
}

TEST_F(FrameBufferTest, DrawFrameBuffer1CA){
    auto fb = createFrameBuffer1CA();
    auto ss = createScreenShader1CA();
    auto va = createScreenQuad(ss);

    fb->bind();
    ss.shader->bind();
    TestEnvironment::m_Context->draw(va);
    fb->unbind();

    auto ca1 = fb->getColorAttachment(0);
    auto img1 = ca1->getData();
    EXPECT_TRUE(isPixelCorrect<float>(img1, 1.0f, 0.0f, 0.0f, 1.0f));
}

TEST_F(FrameBufferTest, DrawFrameBuffer2CA){
    auto fb = createFrameBuffer2CA();
    auto ss = createScreenShader2CA();
    auto va = createScreenQuad(ss);

    fb->bind();
    ss.shader->bind();
    TestEnvironment::m_Context->draw(va);
    fb->unbind();

    auto ca1 = fb->getColorAttachment(0);
    auto img1 = ca1->getData();
    EXPECT_TRUE(isPixelCorrect<float>(img1, 1.0f, 0.0f, 0.0f, 1.0f));

    auto ca2 = fb->getColorAttachment(1);
    auto img2 = ca2->getData();
    EXPECT_TRUE(isPixelCorrect<uint8>(img2, 0, 255, 0, 255));
}