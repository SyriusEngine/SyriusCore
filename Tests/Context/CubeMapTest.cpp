#include "CubeMapTest.hpp"

void CubeMapTest::SetUp() {
    Test::SetUp();
}

void CubeMapTest::TearDown() {
    Test::TearDown();
}

constexpr u32 s_Width = 50;
constexpr u32 s_Height = 50;

template<typename T>
void createVector(std::vector<T>& vec, T red, T green, T blue, T alpha){
    vec.resize(s_Width * s_Height * 4);
    for (size_t i = 0; i < vec.size(); i += 4){
        vec[i] = red;
        vec[i + 1] = green;
        vec[i + 2] = blue;
        vec[i + 3] = alpha;
    }
}

ResourceView<CubeMap> CubeMapTest::createCubeMap() {
    std::vector<u8> red;
    createVector(red, static_cast<u8>(255), static_cast<u8>(0), static_cast<u8>(0), static_cast<u8>(255));
    std::vector<u8> green;
    createVector(green, static_cast<u8>(0), static_cast<u8>(255), static_cast<u8>(0), static_cast<u8>(255));
    std::vector<u8> blue;
    createVector(blue, static_cast<u8>(0), static_cast<u8>(0), static_cast<u8>(255), static_cast<u8>(255));
    std::vector<u8> redBlue;
    createVector(redBlue, static_cast<u8>(255), static_cast<u8>(0), static_cast<u8>(255), static_cast<u8>(255));
    std::vector<u8> greenBlue;
    createVector(greenBlue, static_cast<u8>(0), static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(255));
    std::vector<u8> redGreen;
    createVector(redGreen, static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(0), static_cast<u8>(255));

    ImageUI8Desc desc;
    desc.width = s_Width;
    desc.height = s_Height;
    desc.format = SR_TEXTURE_RGBA_UI8;

    desc.data = red.data();
    auto right = createImage(desc);
    desc.data = blue.data();
    auto left = createImage(desc);
    desc.data = green.data();
    auto top = createImage(desc);
    desc.data = redBlue.data();
    auto bottom = createImage(desc);
    desc.data = redGreen.data();
    auto front = createImage(desc);
    desc.data = greenBlue.data();
    auto back = createImage(desc);

    auto cbl = TestEnvironment::m_Context->createCubeMapLayout(desc.width, desc.height, desc.format);
    cbl->addFace(SR_CUBEMAP_FACE_RIGHT, right);
    cbl->addFace(SR_CUBEMAP_FACE_LEFT, left);
    cbl->addFace(SR_CUBEMAP_FACE_TOP, top);
    cbl->addFace(SR_CUBEMAP_FACE_BOTTOM, bottom);
    cbl->addFace(SR_CUBEMAP_FACE_FRONT, front);
    cbl->addFace(SR_CUBEMAP_FACE_BACK, back);

    auto cb = TestEnvironment::m_Context->createCubeMap(cbl);
    return cb;
}

ResourceView<CubeMap> CubeMapTest::createRedCubeMap() {
    std::vector<u8> red;
    createVector(red, static_cast<u8>(255), static_cast<u8>(0), static_cast<u8>(0), static_cast<u8>(255));

    ImageUI8Desc desc;
    desc.width = s_Width;
    desc.height = s_Height;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = red.data();

    auto right = createImage(desc);
    auto left = createImage(desc);
    auto top = createImage(desc);
    auto bottom = createImage(desc);
    auto front = createImage(desc);
    auto back = createImage(desc);

    auto cbl = TestEnvironment::m_Context->createCubeMapLayout(desc.width, desc.height, desc.format);
    cbl->addFace(SR_CUBEMAP_FACE_RIGHT, right);
    cbl->addFace(SR_CUBEMAP_FACE_LEFT, left);
    cbl->addFace(SR_CUBEMAP_FACE_TOP, top);
    cbl->addFace(SR_CUBEMAP_FACE_BOTTOM, bottom);
    cbl->addFace(SR_CUBEMAP_FACE_FRONT, front);
    cbl->addFace(SR_CUBEMAP_FACE_BACK, back);

    auto cb = TestEnvironment::m_Context->createCubeMap(cbl);
    return cb;
}

TEST_F(CubeMapTest, CreateCubeMapUI8){
    auto cb = createCubeMap();
    EXPECT_NE(cb, nullptr);
    EXPECT_EQ(cb->getWidth(), s_Width);
    EXPECT_EQ(cb->getHeight(), s_Height);
    EXPECT_EQ(cb->getFormat(), SR_TEXTURE_RGBA_UI8);
}

TEST_F(CubeMapTest, CreateCubeMapNoData){
    ImageUI8Desc desc;
    desc.width = s_Width;
    desc.height = s_Height;
    desc.format = SR_TEXTURE_RGBA_UI8;
    desc.data = nullptr;
    auto right = createImage(desc);
    auto left = createImage(desc);
    auto top = createImage(desc);
    auto bottom = createImage(desc);
    auto front = createImage(desc);
    auto back = createImage(desc);

    auto cbl = TestEnvironment::m_Context->createCubeMapLayout(desc.width, desc.height, desc.format);
    cbl->addFace(SR_CUBEMAP_FACE_RIGHT, right);
    cbl->addFace(SR_CUBEMAP_FACE_LEFT, left);
    cbl->addFace(SR_CUBEMAP_FACE_TOP, top);
    cbl->addFace(SR_CUBEMAP_FACE_BOTTOM, bottom);
    cbl->addFace(SR_CUBEMAP_FACE_FRONT, front);
    cbl->addFace(SR_CUBEMAP_FACE_BACK, back);

    auto cb = TestEnvironment::m_Context->createCubeMap(cbl);
    EXPECT_NE(cb, nullptr);
    EXPECT_EQ(cb->getWidth(), desc.width);
    EXPECT_EQ(cb->getHeight(), desc.height);
    EXPECT_EQ(cb->getFormat(), desc.format);
}

bool compareData(const UByte* data, const std::vector<UByte>& vec){
    for (size_t i = 0; i < vec.size(); ++i){
        if (data[i] != vec[i]){
            return false;
        }
    }
    return true;
}

TEST_F(CubeMapTest, ReadCubeMap){
    std::vector<u8> red;
    createVector(red, static_cast<u8>(255), static_cast<u8>(0), static_cast<u8>(0), static_cast<u8>(255));
    std::vector<u8> green;
    createVector(green, static_cast<u8>(0), static_cast<u8>(255), static_cast<u8>(0), static_cast<u8>(255));
    std::vector<u8> blue;
    createVector(blue, static_cast<u8>(0), static_cast<u8>(0), static_cast<u8>(255), static_cast<u8>(255));
    std::vector<u8> redBlue;
    createVector(redBlue, static_cast<u8>(255), static_cast<u8>(0), static_cast<u8>(255), static_cast<u8>(255));
    std::vector<u8> greenBlue;
    createVector(greenBlue, static_cast<u8>(0), static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(255));
    std::vector<u8> redGreen;
    createVector(redGreen, static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(0), static_cast<u8>(255));

    auto cb = createCubeMap();

    auto img = cb->getData(SR_CUBEMAP_FACE_RIGHT);
    EXPECT_TRUE(compareData(reinterpret_cast<const UByte*>(img->getData()), red));
    img = cb->getData(SR_CUBEMAP_FACE_LEFT);
    EXPECT_TRUE(compareData(reinterpret_cast<const UByte*>(img->getData()), blue));
    img = cb->getData(SR_CUBEMAP_FACE_TOP);
    EXPECT_TRUE(compareData(reinterpret_cast<const UByte*>(img->getData()), green));
    img = cb->getData(SR_CUBEMAP_FACE_BOTTOM);
    EXPECT_TRUE(compareData(reinterpret_cast<const UByte*>(img->getData()), redBlue));
    img = cb->getData(SR_CUBEMAP_FACE_FRONT);
    EXPECT_TRUE(compareData(reinterpret_cast<const UByte*>(img->getData()), redGreen));
    img = cb->getData(SR_CUBEMAP_FACE_BACK);
    EXPECT_TRUE(compareData(reinterpret_cast<const UByte*>(img->getData()), greenBlue));
}

// TEST_F(CubeMapTest, CopyFromTextureCubeMap){
//     // first create cubemap
//     auto redCb = createRedCubeMap();
//
//     // then create texture
//     std::vector<u8> blue;
//     createVector(blue, static_cast<u8>(0), static_cast<u8>(0), static_cast<u8>(255), static_cast<u8>(255));
//
//     Texture2DDesc desc;
//     desc.width = s_Width;
//     desc.height = s_Height;
//     desc.format = SR_TEXTURE_RGBA_UI8;
//     desc.data = blue.data();
//     desc.usage = SR_BUFFER_USAGE_DEFAULT;
//     auto tex = TestEnvironment::m_Context->createTexture2D(desc);
//
//     // update top face of cubemap with texture data
//     redCb->copyFrom(tex, SR_CUBEMAP_FACE_LEFT);
//     redCb->copyFrom(tex, SR_CUBEMAP_FACE_RIGHT);
//     redCb->copyFrom(tex, SR_CUBEMAP_FACE_TOP);
//     redCb->copyFrom(tex, SR_CUBEMAP_FACE_BOTTOM);
//     redCb->copyFrom(tex, SR_CUBEMAP_FACE_FRONT);
//     redCb->copyFrom(tex, SR_CUBEMAP_FACE_BACK);
//
//
//     // validate
//     auto img = redCb->getData(SR_CUBEMAP_FACE_TOP);
//     EXPECT_TRUE(compareData(reinterpret_cast<const UByte*>(img->getData()), blue));
// }