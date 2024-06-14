#include "ConstantBufferTest.hpp"

void ConstantBufferTest::SetUp() {
    m_Data.position.x = 1.0f;
    m_Data.position.y = 2.0f;
    m_Data.position.z = 3.0f;
    m_Data.position.w = 4.0f;

    for (int i = 0; i < 4; i++) {
        m_Data.view.rows[i].x = i;
        m_Data.view.rows[i].y = i + 1;
        m_Data.view.rows[i].z = i + 2;
        m_Data.view.rows[i].w = i + 3;
    }
}

void ConstantBufferTest::TearDown() {
    Test::TearDown();
}

TEST_F(ConstantBufferTest, CreateConstantBufferVertex){
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &m_Data;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;

    auto cb = TestEnvironment::m_Context->createConstantBuffer(desc);
    EXPECT_NE(cb, nullptr);
}

TEST_F(ConstantBufferTest, CreateConstantBufferFragment){
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &m_Data;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_FRAGMENT;

    auto cb = TestEnvironment::m_Context->createConstantBuffer(desc);
    EXPECT_NE(cb, nullptr);
}

TEST_F(ConstantBufferTest, CreateConstantBufferZeroSize){
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = 0;
    desc.data = nullptr;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;

    EXPECT_DEATH(auto cb = TestEnvironment::m_Context->createConstantBuffer(desc), "");
}

TEST_F(ConstantBufferTest, CreateConstantBufferEmptyData){
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = nullptr;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;

    auto cb = TestEnvironment::m_Context->createConstantBuffer(desc);
    EXPECT_NE(cb, nullptr);
    EXPECT_EQ(cb->getSize(), sizeof(CBData));
}

TEST_F(ConstantBufferTest, CreateConstantBufferInvalidAlignment){
    float data[3] = {1.0f, 2.0f, 3.0f};
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(data);
    desc.data = data;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;

    EXPECT_DEATH(auto cb = TestEnvironment::m_Context->createConstantBuffer(desc), "");
}

TEST_F(ConstantBufferTest, ReadConstantBuffer){
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &m_Data;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;

    auto cb = TestEnvironment::m_Context->createConstantBuffer(desc);
    EXPECT_NE(cb, nullptr);

    auto data = cb->getData();
    auto cbData = reinterpret_cast<CBData*>(data.get());
    EXPECT_EQ(cbData->position.x, 1.0f);
    EXPECT_EQ(cbData->position.y, 2.0f);
    EXPECT_EQ(cbData->position.z, 3.0f);
    EXPECT_EQ(cbData->position.w, 4.0f);

    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(cbData->view.rows[i].x, i);
        EXPECT_EQ(cbData->view.rows[i].y, i + 1);
        EXPECT_EQ(cbData->view.rows[i].z, i + 2);
        EXPECT_EQ(cbData->view.rows[i].w, i + 3);
    }
}

TEST_F(ConstantBufferTest, UpdateConstantBuffer){
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &m_Data;
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.shaderStage = SR_SHADER_VERTEX;

    auto cb = TestEnvironment::m_Context->createConstantBuffer(desc);

    CBData newData = m_Data;
    newData.position.x = 10.0f;
    newData.position.y = 20.0f;
    newData.position.z = 30.0f;
    newData.position.w = 40.0f;

    cb->setData(&newData, sizeof(CBData));

    auto data = cb->getData();
    auto cbData = reinterpret_cast<CBData*>(data.get());
    EXPECT_EQ(cbData->position.x, 10.0f);
    EXPECT_EQ(cbData->position.y, 20.0f);
    EXPECT_EQ(cbData->position.z, 30.0f);
    EXPECT_EQ(cbData->position.w, 40.0f);

    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(cbData->view.rows[i].x, i);
        EXPECT_EQ(cbData->view.rows[i].y, i + 1);
        EXPECT_EQ(cbData->view.rows[i].z, i + 2);
        EXPECT_EQ(cbData->view.rows[i].w, i + 3);
    }
}

TEST_F(ConstantBufferTest, UpdateConstantBufferStatic){
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &m_Data;
    desc.usage = SR_BUFFER_USAGE_STATIC;
    desc.shaderStage = SR_SHADER_VERTEX;

    auto cb = TestEnvironment::m_Context->createConstantBuffer(desc);

    EXPECT_DEATH(cb->setData(&m_Data, sizeof(CBData)), "");
}

TEST_F(ConstantBufferTest, UpdateConstantBufferLargerData){
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &m_Data;
    desc.usage = SR_BUFFER_USAGE_STATIC;
    desc.shaderStage = SR_SHADER_VERTEX;

    auto cb = TestEnvironment::m_Context->createConstantBuffer(desc);

    CBData newData[2];
    newData[0] = m_Data;
    newData[1] = m_Data;
    EXPECT_DEATH(cb->setData(newData, sizeof(CBData) * 2), "");
}

TEST_F(ConstantBufferTest, CopyConstantBuffer){
    ConstantBufferDesc desc;
    desc.name = "CBData";
    desc.size = sizeof(CBData);
    desc.data = &m_Data;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;

    auto srcCb = TestEnvironment::m_Context->createConstantBuffer(desc);

    desc.data = nullptr;
    auto dstCb = TestEnvironment::m_Context->createConstantBuffer(desc);

    dstCb->copyFrom(srcCb);

    auto data = dstCb->getData();
    auto cbData = reinterpret_cast<CBData*>(data.get());
    EXPECT_EQ(cbData->position.x, 1.0f);
    EXPECT_EQ(cbData->position.y, 2.0f);
    EXPECT_EQ(cbData->position.z, 3.0f);
    EXPECT_EQ(cbData->position.w, 4.0f);

    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(cbData->view.rows[i].x, i);
        EXPECT_EQ(cbData->view.rows[i].y, i + 1);
        EXPECT_EQ(cbData->view.rows[i].z, i + 2);
        EXPECT_EQ(cbData->view.rows[i].w, i + 3);
    }

}
