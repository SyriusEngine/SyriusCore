#include "../TestEnvironment.hpp"

struct SomeData {
    float mat4[16];
};

class ShaderStorageBufferTest: public ::testing::Test {
protected:
    void SetUp() override {
        for (int i = 0; i < 16; i++) {
            data.mat4[i] = static_cast<float>(i);
        }

    }

    void TearDown() override {

    }

protected:
    SomeData data = {};
};

TEST_F(ShaderStorageBufferTest, CreateSSBOVertex) {
    ShaderStorageBufferDesc desc;
    desc.name = "SSBOTest";
    desc.data = &data;
    desc.size = sizeof(SomeData);
    desc.count = 1;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;

    auto ssbo = TestEnvironment::m_Context->createShaderStorageBuffer(desc);
    EXPECT_EQ(ssbo->getName(), desc.name);
    EXPECT_EQ(ssbo->getSize(), desc.size);
    EXPECT_EQ(ssbo->getUsage(), desc.usage);
    EXPECT_EQ(ssbo->getCount(), desc.count);
}

TEST_F(ShaderStorageBufferTest, CreateSSBOFragment) {
    ShaderStorageBufferDesc desc;
    desc.name = "SSBOTest";
    desc.data = &data;
    desc.size = sizeof(SomeData);
    desc.count = 1;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_FRAGMENT;

    auto ssbo = TestEnvironment::m_Context->createShaderStorageBuffer(desc);
    EXPECT_EQ(ssbo->getName(), desc.name);
    EXPECT_EQ(ssbo->getSize(), desc.size);
    EXPECT_EQ(ssbo->getUsage(), desc.usage);
    EXPECT_EQ(ssbo->getCount(), desc.count);
}

TEST_F(ShaderStorageBufferTest, GetData) {
    ShaderStorageBufferDesc desc;
    desc.name = "SSBOTest";
    desc.data = &data;
    desc.size = sizeof(SomeData);
    desc.count = 1;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;

    auto ssbo = TestEnvironment::m_Context->createShaderStorageBuffer(desc);
    auto data = reinterpret_cast<SomeData*>(ssbo->getData().get());
    for (int i = 0; i < 16; i++) {
        EXPECT_NEAR(data->mat4[i], static_cast<float>(i), 1e-4);
    }
}
