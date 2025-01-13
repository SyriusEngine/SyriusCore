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