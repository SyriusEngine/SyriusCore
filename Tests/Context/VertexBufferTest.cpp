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
    desc.data = s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.layout = m_Layout;

    auto vb = TestEnvironment::m_Context->createVertexBuffer(desc);

    EXPECT_NE(vb, nullptr);
    EXPECT_EQ(vb->getCount(), 3);
    EXPECT_EQ(vb->getSize(), sizeof(float) * s_TriangleVertices.size());
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

TEST_F(VertexBufferTest, CreateVertexBufferEmptyData){
    VertexBufferDesc desc;
    desc.data = nullptr;
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.layout = m_Layout;

    auto vb = TestEnvironment::m_Context->createVertexBuffer(desc);

    EXPECT_NE(vb, nullptr);
    EXPECT_EQ(vb->getCount(), 3);
}

TEST_F(VertexBufferTest, CreateVertexBufferInvalidLayout){
    VertexBufferDesc desc;
    desc.data = s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.layout = ResourceView<VertexLayout>(nullptr);

    EXPECT_DEATH(auto vb = TestEnvironment::m_Context->createVertexBuffer(desc), "");
}

TEST_F(VertexBufferTest, ReadVertexBuffer){
    VertexBufferDesc desc;
    desc.data = s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.layout = m_Layout;

    auto vb = TestEnvironment::m_Context->createVertexBuffer(desc);

    auto data = vb->getData();
    EXPECT_NE(data, nullptr);

    // check if the data has changed
    auto floatData = reinterpret_cast<float*>(data.get());
    for (int i = 0; i < s_TriangleVertices.size(); i++){
        EXPECT_EQ(floatData[i], s_TriangleVertices[i]);
    }
}

TEST_F(VertexBufferTest, UpdateVertexBuffer){
    VertexBufferDesc desc;
    desc.data = s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.layout = m_Layout;

    auto vb = TestEnvironment::m_Context->createVertexBuffer(desc);

    vb->setData(s_TriangleVertices2.data(), 3);

    auto data = vb->getData();
    EXPECT_NE(data, nullptr);

    // check if the data has changed
    auto floatData = reinterpret_cast<float*>(data.get());
    for (int i = 0; i < s_TriangleVertices2.size(); i++){
        EXPECT_EQ(floatData[i], s_TriangleVertices2[i]);
    }
}

TEST_F(VertexBufferTest, UpdateVertexBufferStatic){
    VertexBufferDesc desc;
    desc.data = s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_STATIC; // should not be able to update
    desc.layout = m_Layout;

    auto vb = TestEnvironment::m_Context->createVertexBuffer(desc);

    EXPECT_DEATH(vb->setData(s_TriangleVertices2.data(), 3), "");
}

TEST_F(VertexBufferTest, UpdateVertexBufferLargerData){
    VertexBufferDesc desc;
    desc.data = s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.layout = m_Layout;

    auto vb = TestEnvironment::m_Context->createVertexBuffer(desc);

    EXPECT_DEATH(vb->setData(s_RectangleVertices.data(), 4), "");
}
