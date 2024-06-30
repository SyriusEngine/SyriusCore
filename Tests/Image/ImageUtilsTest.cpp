#include "ImageUtilsTest.hpp"

void ImageUtilsTest::SetUp() {
    Test::SetUp();
}

void ImageUtilsTest::TearDown() {
    Test::TearDown();
}

TEST_F(ImageUtilsTest, GetBytesPerPixelTest){
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_RGBA_UI8), 4);
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_RGB_UI8), 3);
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_RG_UI8), 2);
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_R_UI8), 1);

    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_RGBA_F32), 16);
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_RGB_F32), 12);
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_RG_F32), 8);
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_R_F32), 4);

    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_RGBA_UI16), 8);
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_RGB_UI16), 6);
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_RG_UI16), 4);
    EXPECT_EQ(getBytesPerPixel(SR_TEXTURE_R_UI16), 2);
}

TEST_F(ImageUtilsTest, GetFormatFromChannelCountTest){
    auto v = getFormatFromChannelCount(1, SR_UINT8);
    EXPECT_EQ(getFormatFromChannelCount(1, SR_UINT8), SR_TEXTURE_R_UI8);
    EXPECT_EQ(getFormatFromChannelCount(2, SR_UINT8), SR_TEXTURE_RG_UI8);
    EXPECT_EQ(getFormatFromChannelCount(3, SR_UINT8), SR_TEXTURE_RGB_UI8);
    EXPECT_EQ(getFormatFromChannelCount(4, SR_UINT8), SR_TEXTURE_RGBA_UI8);

    EXPECT_EQ(getFormatFromChannelCount(1, SR_FLOAT32), SR_TEXTURE_R_F32);
    EXPECT_EQ(getFormatFromChannelCount(2, SR_FLOAT32), SR_TEXTURE_RG_F32);
    EXPECT_EQ(getFormatFromChannelCount(3, SR_FLOAT32), SR_TEXTURE_RGB_F32);
    EXPECT_EQ(getFormatFromChannelCount(4, SR_FLOAT32), SR_TEXTURE_RGBA_F32);

    EXPECT_EQ(getFormatFromChannelCount(1, SR_UINT16), SR_TEXTURE_R_UI16);
    EXPECT_EQ(getFormatFromChannelCount(2, SR_UINT16), SR_TEXTURE_RG_UI16);
    EXPECT_EQ(getFormatFromChannelCount(3, SR_UINT16), SR_TEXTURE_RGB_UI16);
    EXPECT_EQ(getFormatFromChannelCount(4, SR_UINT16), SR_TEXTURE_RGBA_UI16);
}