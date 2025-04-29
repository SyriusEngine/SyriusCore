#include <gtest/gtest.h>
#include "../TestEnvironment.hpp"

class ShaderTest: public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(ShaderTest, CreateBasicShader) {
    /*
     * Create a basic shader with a vertex and fragment shader
     */
    auto vDesc = TestEnvironment::createShaderModuleDescFromFileVS("VertexBuffer");
    auto fDesc = TestEnvironment::createShaderModuleDescFromFileFS("VertexBuffer");

    ShaderDesc desc;
    desc.vertexShader = TestEnvironment::m_Context->createShaderModule(vDesc);
    desc.fragmentShader = TestEnvironment::m_Context->createShaderModule(fDesc);

    auto shader = TestEnvironment::m_Context->createShader(desc);
    EXPECT_FALSE(shader == nullptr);
}