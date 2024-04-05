#include "IndexBufferTest.hpp"

void IndexBufferTest::SetUp() {
    Test::SetUp();
}

void IndexBufferTest::TearDown() {
    Test::TearDown();
}

TEST_F(IndexBufferTest, CreateIndexBuffer) {
    IndexBufferDesc desc;
    desc.data = s_TriangleIndices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.dataType = SR_UINT32;

    auto ib = TestEnvironment::m_Context->createIndexBuffer(desc);

    EXPECT_NE(ib, nullptr);
    EXPECT_EQ(ib->getCount(), 3);
    EXPECT_EQ(ib->getSize(), sizeof(uint32) * s_TriangleIndices.size());
    EXPECT_EQ(ib->getUsage(), SR_BUFFER_USAGE_DEFAULT);
    EXPECT_EQ(ib->getDataType(), SR_UINT32);
}

TEST_F(IndexBufferTest, CreateIndexBufferZeroSize){
    IndexBufferDesc desc;
    desc.data = nullptr;
    desc.count = 0;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.dataType = SR_UINT32;

    EXPECT_DEATH(auto ib = TestEnvironment::m_Context->createIndexBuffer(desc), "");
}

TEST_F(IndexBufferTest, CreateIndexBufferEmptyData){
    IndexBufferDesc desc;
    desc.data = nullptr;
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.dataType = SR_UINT32;

    auto ib = TestEnvironment::m_Context->createIndexBuffer(desc);

    EXPECT_NE(ib, nullptr);
    EXPECT_EQ(ib->getCount(), 3);
}

TEST_F(IndexBufferTest, CreateIndexBufferInvalidDataType){
    IndexBufferDesc desc;
    desc.data = nullptr;
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.dataType = SR_VOID;

    EXPECT_DEATH(auto ibVoid = TestEnvironment::m_Context->createIndexBuffer(desc), "");

    desc.dataType = SR_FLOAT16;
    EXPECT_DEATH(auto ibFloat16 = TestEnvironment::m_Context->createIndexBuffer(desc), "");

    desc.dataType = SR_FLOAT32;
    EXPECT_DEATH(auto ibFloat32 = TestEnvironment::m_Context->createIndexBuffer(desc), "");

    desc.dataType = SR_FLOAT64;
    EXPECT_DEATH(auto ibFloat64 = TestEnvironment::m_Context->createIndexBuffer(desc), "");
}

TEST_F(IndexBufferTest, ReadIndexBuffer){
    IndexBufferDesc desc;
    desc.data = s_TriangleIndices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.dataType = SR_UINT32;

    auto ib = TestEnvironment::m_Context->createIndexBuffer(desc);

    auto data = ib->getData();
    EXPECT_NE(data, nullptr);

    // check if the data has changed
    auto uint32Data = reinterpret_cast<uint32*>(data.get());
    for (int i = 0; i < s_TriangleIndices.size(); i++){
        EXPECT_EQ(uint32Data[i], s_TriangleIndices[i]);
    }
}

TEST_F(IndexBufferTest, UpdateIndexBuffer){
    IndexBufferDesc desc;
    desc.data = s_TriangleIndices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.dataType = SR_UINT32;

    auto ib = TestEnvironment::m_Context->createIndexBuffer(desc);

    ib->setData(s_TriangleIndices2.data(), 3);

    auto data = ib->getData();
    EXPECT_NE(data, nullptr);

    // check if the data has changed
    auto uint32Data = reinterpret_cast<uint32*>(data.get());
    for (int i = 0; i < s_TriangleIndices2.size(); i++){
        EXPECT_EQ(uint32Data[i], s_TriangleIndices2[i]);
    }
}

TEST_F(IndexBufferTest, UpdateIndexBufferStatic){
    IndexBufferDesc desc;
    desc.data = s_TriangleIndices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_STATIC;
    desc.dataType = SR_UINT32;

    auto ib = TestEnvironment::m_Context->createIndexBuffer(desc);

    EXPECT_DEATH(ib->setData(s_TriangleIndices2.data(), 3), "");
}

TEST_F(IndexBufferTest, UpdateIndexBufferLargerData){
    IndexBufferDesc desc;
    desc.data = s_TriangleIndices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.dataType = SR_UINT32;

    auto ib = TestEnvironment::m_Context->createIndexBuffer(desc);

    EXPECT_DEATH(ib->setData(s_RectangleIndices.data(), 4), "");
}
