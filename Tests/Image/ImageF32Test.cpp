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
    for (u32 i = 0; i < width * height * channelCount; i += channelCount){
        if (data[i] - 1.0f > epsilon or data[i + 1] > epsilon or data[i + 2] > epsilon){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageF32Test, CreateImageFromFileF32RequestMoreChannels){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-F32-RGB.hdr"; // 3 channels
    desc.requestedChannelCount = 4; // but we want 4
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 100);
    EXPECT_EQ(height, 100);
    EXPECT_EQ(channelCount, 4);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_RGBA_F32);

    auto data = reinterpret_cast<float*>(img->getData());
    auto epsilon = 0.01f;
    bool correct = true;
    for (u32 i = 0; i < width * height * channelCount; i += channelCount){
        if (data[i] - 1.0f > epsilon or data[i + 1] > epsilon or data[i + 2] > epsilon){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageF32Test, CreateImageFromFileF32RequestFewerChannels){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-F32-RGB.hdr"; // 3 channels
    desc.requestedChannelCount = 1; // but we want 1 (only red)
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 100);
    EXPECT_EQ(height, 100);
    EXPECT_EQ(channelCount, 1);
    EXPECT_EQ(img->getFormat(), SR_TEXTURE_R_F32);

    auto data = reinterpret_cast<float*>(img->getData());
    auto epsilon = 0.01f;
    bool correct = true;
    for (u32 i = 0; i < width * height * channelCount; i += channelCount){
        if (data[i] - 1.0f > epsilon){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageF32Test, CreateImageFromMemoryF32WithData){
    std::vector<float> data = {
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };
    ImageF32Desc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGB_F32;
    desc.data = data.data();
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 2);
    EXPECT_EQ(height, 2);
    EXPECT_EQ(channelCount, 3);
    EXPECT_EQ(img->getFormat(), desc.format);

    auto imgData = reinterpret_cast<float*>(img->getData());
    bool correct = true;
    for (u32 i = 0; i < width * height * channelCount; i += channelCount){
        if (imgData[i] != data[i]){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageF32Test, CreateImageFromMemoryF32NoData){
    ImageF32Desc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGB_F32;
    auto img = createImage(desc);

    auto width = img->getWidth();
    auto height = img->getHeight();
    auto channelCount = img->getChannelCount();

    EXPECT_EQ(width, 2);
    EXPECT_EQ(height, 2);
    EXPECT_EQ(channelCount, 3);
    EXPECT_EQ(img->getFormat(), desc.format);

    auto imgData = reinterpret_cast<float*>(img->getData());
    bool correct = true;
    for (u32 i = 0; i < width * height * channelCount; i += channelCount){
        if (imgData[i] != desc.defaultChannelValue){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageF32Test, CreateImageF32InvalidFormat){
    ImageF32Desc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_NONE;
    EXPECT_THROW(createImage(desc), SyriusAssert);
}

TEST_F(ImageF32Test, CreateImageF32FromT){
    std::vector<float> data = {
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };

    ImageDesc desc;
    desc.width = 2;
    desc.height = 2;
    desc.format = SR_TEXTURE_RGB_F32;
    desc.data = data.data();
    auto img = createImage(desc);

    EXPECT_EQ(img->getWidth(), desc.width);
    EXPECT_EQ(img->getHeight(), desc.height);
    EXPECT_EQ(img->getFormat(), desc.format);
}

TEST_F(ImageF32Test, UpscaleImageF32){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-F32-RGB.hdr";
    desc.requestedChannelCount = 0; // 0 means all channels
    auto img = createImage(desc);

    auto newWidth = img->getWidth() * 2;
    auto newHeight = img->getHeight() * 2;
    auto channelCount = img->getChannelCount();
    img->resize(newWidth, newHeight);

    EXPECT_EQ(img->getWidth(), newWidth);
    EXPECT_EQ(img->getHeight(), newHeight);
    EXPECT_EQ(img->getChannelCount(), channelCount);

    auto data = reinterpret_cast<float*>(img->getData());
    auto epsilon = 0.1f;
    bool correct = true;
    for (u32 i = 0; i < newWidth * newHeight * channelCount; i += channelCount){
        if (data[i] - 1.0f > epsilon or data[i + 1] > epsilon or data[i + 2] > epsilon){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}

TEST_F(ImageF32Test, DownscaleImageF32){
    ImageFileDesc desc;
    desc.fileName = "./TestResources/Textures/Red-100x100-F32-RGB.hdr";
    desc.requestedChannelCount = 0; // 0 means all channels
    auto img = createImage(desc);

    auto newWidth = img->getWidth() / 2;
    auto newHeight = img->getHeight() / 2;
    auto channelCount = img->getChannelCount();
    img->resize(newWidth, newHeight);

    EXPECT_EQ(img->getWidth(), newWidth);
    EXPECT_EQ(img->getHeight(), newHeight);
    EXPECT_EQ(img->getChannelCount(), channelCount);

    auto data = reinterpret_cast<float*>(img->getData());
    auto epsilon = 0.1f;
    bool correct = true;
    for (u32 i = 0; i < newWidth * newHeight * channelCount; i += channelCount){
        if (data[i] - 1.0f > epsilon or data[i + 1] > epsilon or data[i + 2] > epsilon){
            correct = false;
            break;
        }
    }
    EXPECT_TRUE(correct);
}


