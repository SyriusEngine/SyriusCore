#include "Texture2DTest.hpp"

void Texture2DTest::SetUp() {
    Test::SetUp();
}

void Texture2DTest::TearDown() {
    Test::TearDown();
}

void createCheckerboard(std::vector<ubyte>& data, ubyte r, ubyte g, ubyte b){
    int32 sw = 0;
    for (size_t i = 0; i < data.size(); i += 4) {
        if (sw % 2 == 0) {
            data[i] = 255;
            data[i + 1] = 0;
            data[i + 2] = 0;
            data[i + 3] = 255;
        } else {
            data[i] = r;
            data[i + 1] = g;
            data[i + 2] = b;
            data[i + 3] = 255;
        }
        sw++;
    }
}

TEST_F(Texture2DTest, CreateTexture2D){
    Texture2DDesc desc;
    desc.width = s_AwesomeFaceWidth;
    desc.height = s_AwesomeFaceHeight;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_AwesomeFaceData.data();
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    EXPECT_NE(tex, nullptr);
    EXPECT_EQ(tex->getWidth(), s_AwesomeFaceWidth);
    EXPECT_EQ(tex->getHeight(), s_AwesomeFaceHeight);
    EXPECT_EQ(tex->getFormat(), SR_TEXTURE_RGBA_UI8);
}

TEST_F(Texture2DTest, ReadTexture2D){
    Texture2DDesc desc;
    desc.width = s_AwesomeFaceWidth;
    desc.height = s_AwesomeFaceHeight;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_AwesomeFaceData.data();
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    auto img = tex->getData();
    EXPECT_EQ(img->getWidth(), s_AwesomeFaceWidth);
    EXPECT_EQ(img->getHeight(), s_AwesomeFaceHeight);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_UI8);

    auto data = img->getData();
    auto ui8Data = reinterpret_cast<const ubyte*>(data);
    bool equal = true;
    for (size_t i = 0; i < s_AwesomeFaceData.size(); ++i){
        if (ui8Data[i] != s_AwesomeFaceData[i]){
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
    Texture2DDesc desc;
    desc.width = s_AwesomeFaceWidth;
    desc.height = s_AwesomeFaceHeight;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_AwesomeFaceData.data();
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    std::vector<ubyte> newData(512 * 512 * 4);
    createCheckerboard(newData, 0, 0, 255);

    // update the texture with a different image with equal dimensions
    tex->setData(newData.data(), 0, 0, 512, 512);

    auto img = tex->getData();
    auto data = img->getData();
    auto ui8Data = reinterpret_cast<const ubyte*>(data);
    bool equal = true;
    for (size_t i = 0; i < s_RedBlue.size(); ++i){
        if (ui8Data[i] != newData[i]){
            equal = false;
            break;
        }
    }
    EXPECT_TRUE(equal);

    if (TestEnvironment::m_ExportImages){
        ImageFileDesc fileDesc;
        fileDesc.fileName = "Texture2DTest_UpdateTexture2DFull.png";
        fileDesc.flipOnAccess = true;
        img->writeToFile(fileDesc);
    }
}

TEST_F(Texture2DTest, UpdateTexture2DPartial){
    Texture2DDesc desc;
    desc.width = s_AwesomeFaceWidth;
    desc.height = s_AwesomeFaceHeight;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = s_AwesomeFaceData.data();
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    // update starting from (0, 0) with a width of 128 and a height of 128
    std::vector<ubyte> redGreen(256 * 256 * 4);
    createCheckerboard(redGreen, 0, 255, 0);
    tex->setData(redGreen.data(), 0, 0, 256, 256);

    // update starting from (128, 128) with a width of 128 and a height of 128
    std::vector<ubyte> redMagenta(256 * 256 * 4);
    createCheckerboard(redMagenta, 255, 0, 255);
    tex->setData(redMagenta.data(), 256, 256, 256, 256);

    auto img = tex->getData();
    auto data = img->getData();
    auto ui8Data = reinterpret_cast<const ubyte*>(data);
    bool equal = true;
    for (size_t i = 0; i < redGreen.size(); ++i){
        std::cout << "Uploaded: " << (int)ui8Data[i] << " Original: " << (int)redGreen[i] << std::endl;
        if (ui8Data[i] != redGreen[i]){
            equal = false;
            break;
        }
    }
    auto offset = redMagenta.size() * 3;
    for (size_t i = 0; i < redMagenta.size(); ++i){
        if (ui8Data[i + offset] != redMagenta[i]){
            equal = false;
            break;
        }
    }
    EXPECT_TRUE(equal);

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