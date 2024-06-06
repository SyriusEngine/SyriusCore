#include "ImageUI8Test.hpp"
#include "../../include/SyriusCore/Image/Image.hpp"

void ImageUI8Test::SetUp() {
    Test::SetUp();
}

void ImageUI8Test::TearDown() {
    Test::TearDown();
}

TEST_F(ImageUI8Test, CreateImageFromFileUI8){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-UI8-RGBA.png";
    desc.requestedChannelCount = 0; // 0 means all channels
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 100);
    EXPECT_EQ(height, 100);
    EXPECT_EQ(channelCount, 4);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_UI8);

    auto data = reinterpret_cast<uint8*>(img->getData());
    bool correct = true;
    for (int i = 0; i < width * height * channelCount; i += channelCount){
        if (data[i] != 255 or data[i + 1] != 0 or data[i + 2]){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}
