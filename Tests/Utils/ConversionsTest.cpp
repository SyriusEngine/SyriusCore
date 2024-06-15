#include "ConversionsTest.hpp"

void Conversions::SetUp() {
    Test::SetUp();
}

void Conversions::TearDown() {
    Test::TearDown();
}

TEST_F(Conversions, testAPIName){
    EXPECT_TRUE(getAPIName(SR_API_OPENGL) == "OpenGL");
    EXPECT_TRUE(getAPIName(SR_API_VULKAN) == "Vulkan");
    EXPECT_TRUE(getAPIName(SR_API_D3D11) == "DirectX 11");
    EXPECT_TRUE(getAPIName(SR_API_D3D12) == "DirectX 12");
    EXPECT_TRUE(getAPIName(SR_API_NONE) == "None");
}

TEST_F(Conversions, testGetTypeSize){
    // Base Types
    EXPECT_EQ(getTypeSize(SR_TYPE_NONE), 0);
    EXPECT_EQ(getTypeSize(SR_VOID), 0);
    EXPECT_EQ(getTypeSize(SR_UINT8), 1);
    EXPECT_EQ(getTypeSize(SR_INT8), 1);
    EXPECT_EQ(getTypeSize(SR_UINT16), 2);
    EXPECT_EQ(getTypeSize(SR_INT16), 2);
    EXPECT_EQ(getTypeSize(SR_UINT32), 4);
    EXPECT_EQ(getTypeSize(SR_INT32), 4);
    EXPECT_EQ(getTypeSize(SR_UINT64), 8);
    EXPECT_EQ(getTypeSize(SR_INT64), 8);
    EXPECT_EQ(getTypeSize(SR_FLOAT16), 2);
    EXPECT_EQ(getTypeSize(SR_FLOAT32), 4);
    EXPECT_EQ(getTypeSize(SR_FLOAT64), 8);

    // Scalar Types, test a few
    EXPECT_EQ(getTypeSize(SR_UINT8_4), 4);
    EXPECT_EQ(getTypeSize(SR_UINT8_3X3), 9);
    EXPECT_EQ(getTypeSize(SR_UINT32_4X4), 64);
    EXPECT_EQ(getTypeSize(SR_INT16_1), 2);
    EXPECT_EQ(getTypeSize(SR_INT16_3), 6);
    EXPECT_EQ(getTypeSize(SR_INT16_4X4), 32);
    EXPECT_EQ(getTypeSize(SR_FLOAT32_1), 4);
    EXPECT_EQ(getTypeSize(SR_FLOAT32_2), 8);
    EXPECT_EQ(getTypeSize(SR_FLOAT32_3), 12);
    EXPECT_EQ(getTypeSize(SR_FLOAT32_4), 16);
    EXPECT_EQ(getTypeSize(SR_FLOAT32_3X3), 36);
    EXPECT_EQ(getTypeSize(SR_FLOAT32_4X4), 64);
    EXPECT_EQ(getTypeSize(SR_FLOAT64_1), 8);
    EXPECT_EQ(getTypeSize(SR_FLOAT64_2), 16);
    EXPECT_EQ(getTypeSize(SR_FLOAT64_3), 24);
    EXPECT_EQ(getTypeSize(SR_FLOAT64_4), 32);
    EXPECT_EQ(getTypeSize(SR_FLOAT64_3X3), 72);
    EXPECT_EQ(getTypeSize(SR_FLOAT64_4X4), 128);
}

TEST_F(Conversions, testGetScalarComponentCount){
    // Scalar Types, test a few
    EXPECT_EQ(getScalarComponentCount(SR_UINT8_4), 4);
    EXPECT_EQ(getScalarComponentCount(SR_UINT8_3X3), 9);
    EXPECT_EQ(getScalarComponentCount(SR_UINT32_4X4), 16);
    EXPECT_EQ(getScalarComponentCount(SR_INT16_1), 1);
    EXPECT_EQ(getScalarComponentCount(SR_INT16_3), 3);
    EXPECT_EQ(getScalarComponentCount(SR_INT16_4X4), 16);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT32_1), 1);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT32_2), 2);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT32_3), 3);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT32_4), 4);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT32_3X3), 9);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT32_4X4), 16);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT64_1), 1);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT64_2), 2);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT64_3), 3);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT64_4), 4);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT64_3X3), 9);
    EXPECT_EQ(getScalarComponentCount(SR_FLOAT64_4X4), 16);
}

TEST_F(Conversions, testGetScalarType){
    EXPECT_EQ(getScalarType(SR_UINT8_4), SR_UINT8);
    EXPECT_EQ(getScalarType(SR_UINT8_3X3), SR_UINT8);
    EXPECT_EQ(getScalarType(SR_UINT32_4X4), SR_UINT32);
    EXPECT_EQ(getScalarType(SR_INT16_1), SR_INT16);
    EXPECT_EQ(getScalarType(SR_INT16_3), SR_INT16);
    EXPECT_EQ(getScalarType(SR_UINT16_4X4), SR_UINT16);
    EXPECT_EQ(getScalarType(SR_FLOAT32_1), SR_FLOAT32);
    EXPECT_EQ(getScalarType(SR_FLOAT32_2), SR_FLOAT32);
    EXPECT_EQ(getScalarType(SR_FLOAT32_3), SR_FLOAT32);
    EXPECT_EQ(getScalarType(SR_FLOAT16_4), SR_FLOAT16);
    EXPECT_EQ(getScalarType(SR_FLOAT64_3X3), SR_FLOAT64);
}

TEST_F(Conversions, testGetTextureChannelCount){
    EXPECT_EQ(getTextureChannelCount(SR_TEXTURE_RGBA_F32), 4);
    EXPECT_EQ(getTextureChannelCount(SR_TEXTURE_RG_I32), 2);
    EXPECT_EQ(getTextureChannelCount(SR_TEXTURE_R_F16), 1);
    EXPECT_EQ(getTextureChannelCount(SR_TEXTURE_RGBA_UI8), 4);

    EXPECT_EQ(getTextureChannelCount(SR_TEXTURE_RGB_UI32), 3);
    EXPECT_EQ(getTextureChannelCount(SR_TEXTURE_RG_UI8), 2);
    EXPECT_EQ(getTextureChannelCount(SR_TEXTURE_R_F32), 1);
}

TEST_F(Conversions, testGetTextureDataType){
    EXPECT_EQ(getTextureDataType(SR_TEXTURE_RGBA_F32), SR_FLOAT32);
    EXPECT_EQ(getTextureDataType(SR_TEXTURE_RG_I32), SR_INT32);
    EXPECT_EQ(getTextureDataType(SR_TEXTURE_R_F16), SR_FLOAT16);
    EXPECT_EQ(getTextureDataType(SR_TEXTURE_RGBA_UI8), SR_UINT8);

    EXPECT_EQ(getTextureDataType(SR_TEXTURE_RGB_UI32), SR_UINT32);
    EXPECT_EQ(getTextureDataType(SR_TEXTURE_RG_UI8), SR_UINT8);
    EXPECT_EQ(getTextureDataType(SR_TEXTURE_R_F32), SR_FLOAT32);
}