#include "CubeMapLayoutTest.hpp"

void CubeMapLayoutTest::SetUp() {
    Test::SetUp();
}

void CubeMapLayoutTest::TearDown() {
    Test::TearDown();
}

TEST_F(CubeMapLayoutTest, CreateCubeMapLayout){
    const uint32 width = 512;
    const uint32 height = 512;
    const SR_TEXTURE_FORMAT format = SR_TEXTURE_RGBA_UI8;

    auto res = TestEnvironment::m_Context->createCubeMapLayout(width, height, format);

    EXPECT_EQ(res->getWidth(), width);
    EXPECT_EQ(res->getHeight(), height);
    EXPECT_EQ(res->getFormat(), format);
    auto& faces = res->getFaces();
    for (const auto& face : faces){
        EXPECT_EQ(face, nullptr);
    }
}

TEST_F(CubeMapLayoutTest, CreateCubeMapLayoutAddFace){
    const uint32 width = 512;
    const uint32 height = 512;
    const SR_TEXTURE_FORMAT format = SR_TEXTURE_RGBA_UI8;

    auto res = TestEnvironment::m_Context->createCubeMapLayout(width, height, format);

    std::vector<uint8> data(width * height * 4, 255);
    ImageUI8Desc desc;
    desc.width = width;
    desc.height = height;
    desc.format = format;
    desc.data = data.data();
    auto img = createImage(desc);

    res->addFace(SR_CUBEMAP_FACE_BACK, img);

    auto& faces = res->getFaces();
    for (const auto& face : faces){
        if (face != nullptr){
            EXPECT_EQ(face->getWidth(), width);
            EXPECT_EQ(face->getHeight(), height);
            EXPECT_EQ(face->getFormat(), format);
        }
    }
}

TEST_F(CubeMapLayoutTest, CreateCubeMapAddFaceInvalidLayout){
    const uint32 width = 512;
    const uint32 height = 512;
    const SR_TEXTURE_FORMAT format = SR_TEXTURE_RGBA_UI8;

    auto res = TestEnvironment::m_Context->createCubeMapLayout(width, height, format);

    std::vector<uint8> data(width * height * 3, 255);
    ImageUI8Desc desc;
    desc.width = width;
    desc.height = height;
    desc.format = SR_TEXTURE_RGB_UI8;
    desc.data = data.data();
    auto img = createImage(desc);

    res->addFace(SR_CUBEMAP_FACE_BACK, img);
    auto& faces = res->getFaces();
    for (const auto& face : faces){
        EXPECT_EQ(face, nullptr);
    }
}

TEST_F(CubeMapLayoutTest, CreateCubeMapAddFaceOtherDimensions){
    const uint32 width = 512;
    const uint32 height = 512;
    const SR_TEXTURE_FORMAT format = SR_TEXTURE_RGBA_UI8;

    auto res = TestEnvironment::m_Context->createCubeMapLayout(width, height, format);

    std::vector<uint8> data((width / 2) * (height / 1) * 4, 255);
    ImageUI8Desc desc;
    desc.width = width / 2;
    desc.height = height / 2;
    desc.format = format;
    desc.data = data.data();
    auto img = createImage(desc);

    res->addFace(SR_CUBEMAP_FACE_BACK, img);

    auto& backFace = res->getFace(SR_CUBEMAP_FACE_BACK);
    EXPECT_EQ(backFace->getWidth(), width);
    EXPECT_EQ(backFace->getHeight(), height);
}
