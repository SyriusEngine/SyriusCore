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
    desc.fileName = "./TestResources/Textures/Red-100x100-UI8-RGB.png";
    desc.requestedChannelCount = 0; // 0 means all channels
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 100);
    EXPECT_EQ(height, 100);
    EXPECT_EQ(channelCount, 3);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGB_UI8);

    auto data = reinterpret_cast<u8*>(img->getData());
    bool correct = true;
    for (u32 i = 0; i < width * height * channelCount; i += channelCount){
        if (data[i] != 255 or data[i + 1] != 0){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageUI8Test, CreateImageFromFileUI8RequestMoreChannels){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-UI8-RGB.png"; // 3 channels
    desc.requestedChannelCount = 4; // but we want 4
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 100);
    EXPECT_EQ(height, 100);
    EXPECT_EQ(channelCount, 4);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_UI8);

    auto data = reinterpret_cast<u8*>(img->getData());
    bool correct = true;
    for (u32 i = 0; i < width * height * channelCount; i += channelCount){
        if (data[i] != 255 or data[i + 1] != 0){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageUI8Test, CreateImageFromFileUI8RequestFewerChannels){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-UI8-RGB.png"; // 3 channels
    desc.requestedChannelCount = 1; // but we want 1 (only red)
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 100);
    EXPECT_EQ(height, 100);
    EXPECT_EQ(channelCount, 1);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_R_UI8);

    auto data = reinterpret_cast<u8*>(img->getData());
    bool correct = true;
    for (u32 i = 0; i < width * height * channelCount; i++){
        if (data[i] != 255){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageUI8Test, CreateImageFromMemoryUI8WithData){
    std::vector<u8> data = {
            255, 0, 255, 255, 0, 255,
            255, 0, 255, 255, 0, 255
    };
    ImageUI8Desc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGB_UI8;
    desc.data = data.data();
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 2);
    EXPECT_EQ(height, 2);
    EXPECT_EQ(channelCount, 3);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGB_UI8);

    auto imgData = reinterpret_cast<u8*>(img->getData());
    bool correct = true;
    for (u32 i = 0; i < width * height * channelCount; i += channelCount){
        if (imgData[i] != data[i]){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageUI8Test, CreateImageFromMemoryUI8NoData){
    ImageUI8Desc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGB_UI8;
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 2);
    EXPECT_EQ(height, 2);
    EXPECT_EQ(channelCount, 3);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGB_UI8);

    auto imgData = reinterpret_cast<u8*>(img->getData());
    bool correct = true;
    for (u32 i = 0; i < width * height * channelCount; i += channelCount){
        if (imgData[i] != desc.defaultChannelValue){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageUI8Test, CreateImageUI8InvalidFormat){
    ImageUI8Desc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_NONE;
    EXPECT_DEATH(createImage(desc), "");
}

TEST_F(ImageUI8Test, CreateImageUI8FromT){
    std::vector<u8> data = {
            255, 0, 255, 255, 0, 255,
            255, 0, 255, 255, 0, 255
    };

    ImageDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGB_UI8;
    desc.data = data.data();
    auto img = createImage(desc);

    EXPECT_EQ(img->getWidth(), desc.width);
    EXPECT_EQ(img->getHeight(), desc.height);
    EXPECT_EQ(img->getFormat(), desc.format);
}

TEST_F(ImageUI8Test, UpscaleImageUI8){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-UI8-RGB.png";
    desc.requestedChannelCount = 0; // 0 means all channels
    auto img = createImage(desc);

    auto newWidth = img->getWidth() * 2;
    auto newHeight = img->getHeight() * 2;
    auto channelCount = img->getChannelCount();
    img->resize(newWidth, newHeight);

    EXPECT_EQ(img->getWidth(), newWidth);
    EXPECT_EQ(img->getHeight(), newHeight);
    EXPECT_EQ(img->getChannelCount(), channelCount);

    auto data = reinterpret_cast<u8*>(img->getData());
    bool correct = true;
    for (u32 i = 0; i < newWidth * newHeight * channelCount; i += channelCount){
        if (data[i] != 255 or data[i + 1] != 0){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageUI8Test, DownscaleImageUI8){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-UI8-RGB.png";
    desc.requestedChannelCount = 0; // 0 means all channels
    auto img = createImage(desc);

    auto newWidth = img->getWidth() / 2;
    auto newHeight = img->getHeight() / 2;
    auto channelCount = img->getChannelCount();
    img->resize(newWidth, newHeight);

    EXPECT_EQ(img->getWidth(), newWidth);
    EXPECT_EQ(img->getHeight(), newHeight);
    EXPECT_EQ(img->getChannelCount(), channelCount);

    auto data = reinterpret_cast<u8*>(img->getData());
    bool correct = true;
    for (u32 i = 0; i < newWidth * newHeight * channelCount; i += channelCount){
        if (data[i] != 255 or data[i + 1] != 0){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}
