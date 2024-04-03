#include "VertexBufferTest.hpp"

void VertexBufferTest::SetUp() {
    m_Layout = TestEnvironment::m_Context->createVertexLayout();
    m_Layout->addAttribute("Position", SR_FLOAT32_3);
    m_Layout->addAttribute("Color", SR_FLOAT32_3);
}

void VertexBufferTest::TearDown() {
    Test::TearDown();
}

TEST_F(VertexBufferTest, CreateVertexBuffer) {
    VertexBufferDesc desc;
    desc.data = s_TriangleVertices;
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.layout = m_Layout;

    auto vb = TestEnvironment::m_Context->createVertexBuffer(desc);

    EXPECT_NE(vb, nullptr);
    EXPECT_EQ(vb->getCount(), 3);
    EXPECT_EQ(vb->getSize(), sizeof(s_TriangleVertices));
    EXPECT_EQ(vb->getUsage(), SR_BUFFER_USAGE_DEFAULT);
    EXPECT_EQ(vb->getLayout(), m_Layout);
}

TEST_F(VertexBufferTest, CreateVertexBufferZeroSize){
    VertexBufferDesc desc;
    desc.data = nullptr;
    desc.count = 0;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.layout = m_Layout;

    EXPECT_DEATH(auto vb = TestEnvironment::m_Context->createVertexBuffer(desc), "");
}

TEST_F(VertexBufferTest, CreateVertexBufferInvalidLayout){
    VertexBufferDesc desc;
    desc.data = s_TriangleVertices;
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.layout = ResourceView<VertexLayout>(nullptr);

    EXPECT_DEATH(auto vb = TestEnvironment::m_Context->createVertexBuffer(desc), "");
}
