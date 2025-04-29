#include "VertexLayoutTest.hpp"

void VertexLayoutTest::SetUp() {

}

void VertexLayoutTest::TearDown() {

}

TEST_F(VertexLayoutTest, CreateVertexLayout) {
    auto res =  TestEnvironment::m_Context->createVertexLayout();
    res->addAttribute("Position", SR_FLOAT32_3); // add 12 bytes
    res->addAttribute("Color", SR_FLOAT32_4); // add 16 bytes
    res->addAttribute("TexCoord", SR_FLOAT32_2); // add 8 bytes

    EXPECT_EQ(res->getStride(), 36);
    EXPECT_EQ(res->getAttributes().size(), 3);
    EXPECT_EQ(res->getAttributeCount(), 3);

    EXPECT_TRUE(res->hasAttribute("Position"));
    EXPECT_TRUE(res->hasAttribute("Color"));
    EXPECT_TRUE(res->hasAttribute("TexCoord"));
}

TEST_F(VertexLayoutTest, CreateInvalidLayout) {
    auto res =  TestEnvironment::m_Context->createVertexLayout();
    EXPECT_THROW(res->addAttribute("Invalid", SR_UINT8_3X3), SyriusAssert);
}

TEST_F(VertexLayoutTest, CreateDuplicateAttributes){
    auto res =  TestEnvironment::m_Context->createVertexLayout();
    res->addAttribute("Position", SR_FLOAT32_3);

    EXPECT_THROW(res->addAttribute("Position", SR_FLOAT32_3), SyriusAssert);
}

TEST_F(VertexLayoutTest, SearchNonExisingAttribute){
    auto res =  TestEnvironment::m_Context->createVertexLayout();
    res->addAttribute("Position", SR_FLOAT32_3);

    EXPECT_FALSE(res->hasAttribute("Color"));
}
