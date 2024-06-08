#include "FrameBufferTest.hpp"

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
    ca2Desc.format = SR_TEXTURE_RGBA_F32;
    ca2Desc.width = s_Width;
    ca2Desc.height = s_Height;
    ca2Desc.clearColor[0] = 0.0f;
    ca2Desc.clearColor[1] = 1.0f;
    ca2Desc.clearColor[2] = 0.0f;
    ca2Desc.clearColor[3] = 1.0f;
    fbLayout->addColorAttachmentDesc(ca2Desc);

    return TestEnvironment::m_Context->createFrameBuffer(fbLayout);
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
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_UI8);

    auto data = reinterpret_cast<uint8*>(img->getData());
    bool correct = true;
    for (uint32 i = 0; i < s_Width * s_Height * 4; i+=4){
        if (data[i] != 255 || data[i+1] != 0 || data[i+2] != 0 || data[i+3] != 255){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
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
    EXPECT_EQ(img1->getFormat(), SR_TEXTURE_RGBA_UI8);

    EXPECT_EQ(img2->getWidth(), s_Width);
    EXPECT_EQ(img2->getHeight(), s_Height);
    EXPECT_EQ(img2->getFormat(), SR_TEXTURE_RGBA_UI8);

    auto data = reinterpret_cast<uint8*>(img1->getData());
    bool correct = true;
    for (uint32 i = 0; i < s_Width * s_Height * 4; i+=4){
        if (data[i] != 255 || data[i+1] != 0 || data[i+2] != 0 || data[i+3] != 255){
            correct = false;
            break;
        }
    }

    data = reinterpret_cast<uint8*>(img2->getData());
    for (uint32 i = 0; i < s_Width * s_Height * 4; i+=4){
        if (data[i] != 0 || data[i+1] != 255 || data[i+2] != 0 || data[i+3] != 255){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(FrameBufferTest, GetColorAttachmentOutOfBounds){
    auto fb = createFrameBuffer1CA();

    EXPECT_DEATH(auto val = fb->getColorAttachment(1), "");
}
