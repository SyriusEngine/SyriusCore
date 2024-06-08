#include "FramebufferLayoutTest.hpp"

void FrameBufferLayoutTest::SetUp() {
    Test::SetUp();
}

void FrameBufferLayoutTest::TearDown() {
    Test::TearDown();
}

TEST_F(FrameBufferLayoutTest, CreateFrameBufferLayout){
    ColorAttachmentDesc ca1;
    ca1.width = 800;
    ca1.height = 600;
    ca1.format = SR_TEXTURE_RGBA_UI8;

    DepthStencilAttachmentDesc dsa1;
    dsa1.width = 800;
    dsa1.height = 600;
    dsa1.format = SR_TEXTURE_DEPTH_24_STENCIL_8;

    ViewportDesc vp1;
    vp1.width = 800;
    vp1.height = 600;

    auto fbLayout = TestEnvironment::m_Context->createFrameBufferLayout();
    fbLayout->addColorAttachmentDesc(ca1);
    fbLayout->addDepthStencilAttachmentDesc(dsa1);
    fbLayout->addViewportDesc(vp1);

    auto colorAttachments = fbLayout->getColorAttachmentDesc();
    auto depthStencilAttachments = fbLayout->getDepthStencilAttachmentDesc();
    auto viewports = fbLayout->getViewportDesc();

    EXPECT_TRUE(fbLayout->isValid());

    EXPECT_EQ(colorAttachments.size(), 1);
    EXPECT_EQ(depthStencilAttachments.size(), 1);
    EXPECT_EQ(viewports.size(), 1);

    EXPECT_EQ(colorAttachments[0].width, 800);
    EXPECT_EQ(colorAttachments[0].height, 600);
    EXPECT_EQ(colorAttachments[0].format, SR_TEXTURE_RGBA_UI8);

    EXPECT_EQ(depthStencilAttachments[0].width, 800);
    EXPECT_EQ(depthStencilAttachments[0].height, 600);
    EXPECT_EQ(depthStencilAttachments[0].format, SR_TEXTURE_DEPTH_24_STENCIL_8);

    EXPECT_EQ(viewports[0].width, 800);
    EXPECT_EQ(viewports[0].height, 600);
}

TEST_F(FrameBufferLayoutTest, CreateEmptyFrameBufferLayout){
    auto fbLayout = TestEnvironment::m_Context->createFrameBufferLayout();

    EXPECT_FALSE(fbLayout->isValid());
}

TEST_F(FrameBufferLayoutTest, CreateFrameBufferLayoutTooManyColorAttachments){
    auto fbLayout = TestEnvironment::m_Context->createFrameBufferLayout();
    /*
     * Usually, the maximum supported number of color attachments is 8, but some devices may support more.
     * 32 is an arbitrary number that is guaranteed to be greater than the maximum supported number of color attachments.
     * Unless hardware advances to support more than 32 color attachments, this test should always pass.
     */
    for (uint32 i = 0; i < 32; i++){
        ColorAttachmentDesc ca;
        ca.width = 800;
        ca.height = 600;
        ca.format = SR_TEXTURE_RGBA_UI8;
        fbLayout->addColorAttachmentDesc(ca);
    }
    EXPECT_FALSE(fbLayout->isValid());
}

TEST_F(FrameBufferLayoutTest, CreateFrameBufferLayoutTooManyAttachmentsCube){
    auto fbLayout = TestEnvironment::m_Context->createFrameBufferLayout();

    CubeColorAttachmentDesc ccaDesc;
    fbLayout->addCubeColorAttachmentDesc(ccaDesc);

    /*
     * Similar to the previous test, the maximum number of color attachments is (usually) 8.
     */
    for (uint32 i = 0; i < 4; i++){
        ColorAttachmentDesc ca;
        ca.width = 800;
        ca.height = 600;
        ca.format = SR_TEXTURE_RGBA_UI8;
        fbLayout->addColorAttachmentDesc(ca);
    }
    EXPECT_FALSE(fbLayout->isValid());
}