#include "ImageF32Test.hpp"

#include "../../include/SyriusCore/Image/Image.hpp"

void ImageF32Test::SetUp() {
    Test::SetUp();
}

void ImageF32Test::TearDown() {
    Test::TearDown();
}

TEST_F(ImageF32Test, CreateImageFromFileF32){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-F32-RGB.hdr";
    desc.requestedChannelCount = 0; // 0 means all channels
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 100);
    EXPECT_EQ(height, 100);
    EXPECT_EQ(channelCount, 3);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGB_F32);

    auto data = reinterpret_cast<float*>(img->getData());
    auto epsilon = 0.01f;
    bool correct = true;
    for (int i = 0; i < width * height * channelCount; i += channelCount){
        if (data[i] - 1.0f > epsilon or data[i + 1] > epsilon or data[i + 2] > epsilon){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}
