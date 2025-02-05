#include "ShaderModuleTest.hpp"
#include "../ShaderSource.hpp"

void ShaderModuleTest::SetUp() {

}

void ShaderModuleTest::TearDown() {

}


TEST_F(ShaderModuleTest, CreateShaderModule) {
    ShaderModuleDesc desc;
    desc.entryPoint = "main";
    desc.shaderType = SR_SHADER_VERTEX;
    if (TestEnvironment::m_API == SR_API_OPENGL){
        desc.language = SR_SHADER_LANGUAGE_GLSL;
        desc.code = s_GLSLVertexShaderSource;
    }
    else if (TestEnvironment::m_API == SR_API_D3D11){
        desc.language = SR_SHADER_LANGUAGE_HLSL;
        desc.code = s_HLSLVertexShaderSource;
    }
    else {
        FAIL() << "Unsupported API";
    }

    const auto vsm = TestEnvironment::m_Context->createShaderModule(desc);

    EXPECT_NE(vsm, nullptr);
    EXPECT_EQ(vsm->getShaderModuleType(), SR_SHADER_VERTEX);
    EXPECT_EQ(vsm->getEntryPoint(), "main");
}

TEST_F(ShaderModuleTest, CreateShaderModuleFile){
    const auto desc = TestEnvironment::createShaderModuleDescFromFileVS("VertexBuffer");

    const auto vsm = TestEnvironment::m_Context->createShaderModule(desc);

    EXPECT_NE(vsm, nullptr);
    EXPECT_EQ(vsm->getShaderModuleType(), SR_SHADER_VERTEX);
    EXPECT_EQ(vsm->getEntryPoint(), "main");
}

TEST_F(ShaderModuleTest, CreateShaderInvalidLanguage){
    ShaderModuleDesc desc;
    desc.entryPoint = "main";
    if (TestEnvironment::m_API == SR_API_OPENGL){
        desc.language = SR_SHADER_LANGUAGE_HLSL; // GLSL is expected
    }
    else if (TestEnvironment::m_API == SR_API_D3D11){
        desc.language = SR_SHADER_LANGUAGE_GLSL; // HLSL is expected
    }
    else {
        FAIL() << "Unsupported API";
    }

    EXPECT_DEATH(auto vsm = TestEnvironment::m_Context->createShaderModule(desc), "");
}

TEST_F(ShaderModuleTest, CompilationError) {
    const auto desc = TestEnvironment::createShaderModuleDescFromFileVS("CompilationError");

   EXPECT_THROW(TestEnvironment::m_Context->createShaderModule(desc), std::runtime_error);
}
