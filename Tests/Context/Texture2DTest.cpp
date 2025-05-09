#include "Texture2DTest.hpp"

void Texture2DTest::SetUp() {
    Test::SetUp();
}

void Texture2DTest::TearDown() {
    Test::TearDown();
}

constexpr u32 s_Width = 50;
constexpr u32 s_Height = 50;

template<typename T>
void createRedVector(std::vector<T>& vec){
    vec.resize(s_Width * s_Height * 4);
    for (size_t i = 0; i < vec.size(); i += 4){
        vec[i] = 255;
        vec[i + 1] = 0;
        vec[i + 2] = 0;
        vec[i + 3] = 255;
    }
}

TEST_F(Texture2DTest, CreateTexture2DUI8){
    std::vector<u8> red;
    createRedVector(red);
    Texture2DDesc desc;
    desc.width = s_Width;
    desc.height = s_Height;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = red.data();
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    EXPECT_NE(tex, nullptr);
    EXPECT_EQ(tex->getWidth(), desc.width);
    EXPECT_EQ(tex->getHeight(), desc.height);
    EXPECT_EQ(tex->getFormat(), desc.format);
}

TEST_F(Texture2DTest, CreateTexture2DF32){
    std::vector<float> red;
    createRedVector(red);
    Texture2DDesc desc;
    desc.width = s_Width;
    desc.height = s_Height;
    desc.format = SR_TEXTURE_RGBA_F32;
    desc.data = red.data();
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    EXPECT_NE(tex, nullptr);
    EXPECT_EQ(tex->getWidth(), desc.width);
    EXPECT_EQ(tex->getHeight(), desc.height);
    EXPECT_EQ(tex->getFormat(), desc.format);
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
    EXPECT_EQ(tex->getWidth(), desc.width);
    EXPECT_EQ(tex->getHeight(), desc.height);
    EXPECT_EQ(tex->getFormat(), desc.format);
}

TEST_F(Texture2DTest, ReadTexture2DUI8){
    std::vector<u8> red;
    createRedVector(red);
    Texture2DDesc desc;
    desc.width = s_Width;
    desc.height = s_Height;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = red.data();
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);

    auto img = tex->getData();

    auto data = img->getData();
    auto ui8Data = reinterpret_cast<const UByte*>(data);
    bool equal = true;
    for (size_t i = 0; i < red.size(); ++i){
        if (ui8Data[i] != red[i]){
            equal = false;
            break;
        }
    }
    EXPECT_TRUE(equal);
}

TEST_F(Texture2DTest, UpdateTexture2DLargerData){
    Texture2DDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = nullptr;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);
    std::vector<u8> largeData(4 * 4 * 4, 255);

    EXPECT_THROW(tex->setData(largeData.data(), 0, 0, 3, 3), SyriusAssert);
}

TEST_F(Texture2DTest, UpdateTexture2DOutOfRegion){
    Texture2DDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = nullptr;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto tex = TestEnvironment::m_Context->createTexture2D(desc);
    std::vector<u8> largeData(2 * 2 * 4, 255);
    EXPECT_THROW(tex->setData(largeData.data(), 300, 300, 2, 2), SyriusAssert);
}

TEST_F(Texture2DTest, CopyTexture2D){
    std::vector<u8> red;
    createRedVector(red);
    Texture2DDesc desc;
    desc.width = s_Width;
    desc.height = s_Height;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = red.data();
    desc.usage = SR_BUFFER_USAGE_DEFAULT;

    auto srcTex = TestEnvironment::m_Context->createTexture2D(desc);

    desc.data = nullptr;
    auto dstTex = TestEnvironment::m_Context->createTexture2D(desc);

    dstTex->copyFrom(srcTex);

    auto img = dstTex->getData();

    auto data = img->getData();
    auto ui8Data = reinterpret_cast<const UByte*>(data);
    bool equal = true;
    for (size_t i = 0; i < red.size(); ++i){
        if (ui8Data[i] != red[i]){
            equal = false;
            break;
        }
    }
    EXPECT_TRUE(equal);
}