#include "Texture2DTest.hpp"

void Texture2DTest::SetUp() {
    Test::SetUp();
}

void Texture2DTest::TearDown() {
    Test::TearDown();
}

TEST_F(Texture2DTest, CreateTexture2D){
    Texture2DDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_RedGreen.data();
    desc.usage = SR_BUFFER_USAGE_STATIC;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    EXPECT_NE(tex, nullptr);
    EXPECT_EQ(tex->getWidth(), 2);
    EXPECT_EQ(tex->getHeight(), 2);
    EXPECT_EQ(tex->getFormat(), SR_TEXTURE_RGBA_UI8);
}

TEST_F(Texture2DTest, ReadTexture2D){
    Texture2DDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_RedGreen.data();
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    auto img = tex->getData();
    EXPECT_EQ(img->getWidth(), 2);
    EXPECT_EQ(img->getHeight(), 2);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_UI8);

    auto data = img->getData();
    auto ui8Data = reinterpret_cast<const ubyte*>(data);
    for (size_t i = 0; i < s_RedGreen.size(); ++i){
        EXPECT_EQ(ui8Data[i], s_RedGreen[i]);
    }

    if (TestEnvironment::m_ExportImages){
        ImageFileDesc fileDesc;
        fileDesc.fileName = "Texture2DTest_ReadTexture2D.png";
        fileDesc.flipOnAccess = true;
        img->writeToFile(fileDesc);
    }
}

TEST_F(Texture2DTest, UpdateTexture2DFull){
    Texture2DDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_RedGreen.data();
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    tex->setData(s_RedBlue.data(), 0, 0, 2, 2);

    auto img = tex->getData();
    EXPECT_EQ(img->getWidth(), 2);
    EXPECT_EQ(img->getHeight(), 2);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_UI8);

    auto data = img->getData();
    auto ui8Data = reinterpret_cast<const ubyte*>(data);
    for (size_t i = 0; i < s_RedBlue.size(); ++i){
        EXPECT_EQ(ui8Data[i], s_RedBlue[i]);
    }

    if (TestEnvironment::m_ExportImages){
        ImageFileDesc fileDesc;
        fileDesc.fileName = "Texture2DTest_UpdateTexture2DFull.png";
        fileDesc.flipOnAccess = true;
        img->writeToFile(fileDesc);
    }
}

TEST_F(Texture2DTest, UpdateTexture2DPartial){
    Texture2DDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_RedGreen.data();
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    // update starting from (0, 0) with a width of 1 and a height of 1
    tex->setData(s_Magenta.data(), 0, 0, 1, 1);
    // update starting from (1, 1) with a width of 1 and a height of 1
    tex->setData(s_Turquoise.data(), 1, 1, 1, 1);

    auto img = tex->getData();
    EXPECT_EQ(img->getWidth(), 2);
    EXPECT_EQ(img->getHeight(), 2);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_UI8);

    auto data = img->getData();
    auto ui8Data = reinterpret_cast<const ubyte*>(data);
    uint32 i = 0;
    for (; i < s_Magenta.size(); ++i){
        EXPECT_EQ(ui8Data[i], s_Magenta[i]);
    }
    for (; i < s_RedGreen.size() - 4; ++i){
        EXPECT_EQ(ui8Data[i], s_RedGreen[i]);
    }
    for (; i < s_Turquoise.size(); ++i){
        EXPECT_EQ(ui8Data[i], s_Turquoise[i - 12]);
    }

    if (TestEnvironment::m_ExportImages){
        ImageFileDesc fileDesc;
        fileDesc.fileName = "Texture2DTest_UpdateTexture2DPartial.png";
        fileDesc.flipOnAccess = false;
        img->writeToFile(fileDesc);
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