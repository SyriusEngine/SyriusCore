#include "Texture2DTest.hpp"

void Texture2DTest::SetUp() {
    Test::SetUp();
}

void Texture2DTest::TearDown() {
    Test::TearDown();
}

TEST_F(Texture2DTest, CreateTexture2D){
    std::vector<ubyte> redGreen;
    uint32 width = 4;
    createRedGreenCheckerboard(redGreen, width);
    Texture2DDesc desc;
    desc.width = width;
    desc.height = width;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = redGreen.data();
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    EXPECT_NE(tex, nullptr);
    EXPECT_EQ(tex->getWidth(), width);
    EXPECT_EQ(tex->getHeight(), width);
    EXPECT_EQ(tex->getFormat(), SR_TEXTURE_RGBA_UI8);
}

TEST_F(Texture2DTest, CreateTexture2DNoData){
    Texture2DDesc desc;
    desc.width = 512;
    desc.height = 512;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = nullptr;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    EXPECT_NE(tex, nullptr);
    EXPECT_EQ(tex->getWidth(), 512);
    EXPECT_EQ(tex->getHeight(), 512);
    EXPECT_EQ(tex->getFormat(), SR_TEXTURE_RGBA_UI8);

}

TEST_F(Texture2DTest, ReadTexture2D){
    std::vector<ubyte> redGreen;
    uint32 width = 4;
    createRedGreenCheckerboard(redGreen, width);
    Texture2DDesc desc;
    desc.width = width;
    desc.height = width;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = redGreen.data();
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    auto img = tex->getData();
    EXPECT_EQ(img->getWidth(), width);
    EXPECT_EQ(img->getHeight(), width);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_UI8);

    auto data = img->getData();
    auto ui8Data = reinterpret_cast<const ubyte*>(data);
    bool equal = true;
    for (size_t i = 0; i < redGreen.size(); ++i){
        if (ui8Data[i] != redGreen[i]){
            equal = false;
            break;
        }
    }
    EXPECT_TRUE(equal);

    if (TestEnvironment::m_ExportImages){
        ImageFileDesc fileDesc;
        fileDesc.fileName = "Texture2DTest_ReadTexture2D.png";
        fileDesc.flipOnAccess = true;
        img->writeToFile(fileDesc);
    }
}

TEST_F(Texture2DTest, UpdateTexture2DFull){

    if (TestEnvironment::m_ExportImages){
        ImageFileDesc fileDesc;
        fileDesc.fileName = "Texture2DTest_UpdateTexture2DFull.png";
        fileDesc.flipOnAccess = true;
        // img->writeToFile(fileDesc);
    }
}

TEST_F(Texture2DTest, UpdateTexture2DPartial){

    if (TestEnvironment::m_ExportImages){
        ImageFileDesc fileDesc;
        fileDesc.fileName = "Texture2DTest_UpdateTexture2DPartial.png";
        fileDesc.flipOnAccess = false;
        // img->writeToFile(fileDesc);
    }
}

TEST_F(Texture2DTest, UpdateTexture2DLargerData){
    Texture2DDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_RedGreen.data();
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    EXPECT_DEATH(tex->setData(s_RedGreenBlue.data(), 0, 0, 3, 3), "");
}

TEST_F(Texture2DTest, UpdateTexture2DOutOfRegion){
    Texture2DDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_RedGreen.data();
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    EXPECT_DEATH(tex->setData(s_RedGreen.data(), 300, 300, 2, 2), "");
}

TEST_F(Texture2DTest, UpdateTexture2DStatic){
    Texture2DDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_RedGreen.data();
    desc.usage = SR_BUFFER_USAGE_STATIC;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    EXPECT_DEATH(tex->setData(s_RedGreen.data(), 0, 0, 2, 2), "");
}