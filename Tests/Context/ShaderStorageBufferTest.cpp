#include "ShaderStorageBufferTest.hpp"

void ShaderStorageBufferTest::SetUp() {
    m_Layout = TestEnvironment::m_Context->createVertexLayout();
    m_Layout->addAttribute("Position", SR_FLOAT32_3);
    m_Layout->addAttribute("Color", SR_FLOAT32_3);

    VertexBufferDesc desc;
    desc.data = s_TriangleVertices.data();
    desc.count = 3;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.layout = m_Layout;
    m_VertexBuffer = TestEnvironment::m_Context->createVertexBuffer(desc);

    // Create shaders and VAO for the VertexSSBO tests
    ShaderModuleDesc vvsmDesc;
    vvsmDesc.shaderType = SR_SHADER_VERTEX;
    if (TestEnvironment::m_API == SR_API_OPENGL){
        vvsmDesc.code = s_GlslVertexShaderVertexSSBOSource;
        vvsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
    }
    auto vvsm = TestEnvironment::m_Context->createShaderModule(vvsmDesc);

    ShaderModuleDesc vfsmDesc;
    vfsmDesc.shaderType = SR_SHADER_FRAGMENT;
    if (TestEnvironment::m_API == SR_API_OPENGL){
        vfsmDesc.code = s_GlslFragmentShaderVertexSSBOSource;
        vfsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
    }
    auto vfsm = TestEnvironment::m_Context->createShaderModule(vfsmDesc);

    ShaderDesc shaderDesc;
    shaderDesc.vertexShader = vvsm;
    shaderDesc.fragmentShader = vfsm;
    m_VSSBOShader = TestEnvironment::m_Context->createShader(shaderDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.vertexBuffer = m_VertexBuffer;
    vaoDesc.vertexShader = vvsm;
    m_VSSBOVertexArray = TestEnvironment::m_Context->createVertexArray(vaoDesc);

    // Create shaders and VAO for the FragmentSSBO tests
    ShaderModuleDesc fvsmDesc;
    fvsmDesc.shaderType = SR_SHADER_VERTEX;
    if (TestEnvironment::m_API == SR_API_OPENGL){
        fvsmDesc.code = s_GlslVertexShaderFragmentSSBOSource;
        fvsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
    }
    auto fvsm = TestEnvironment::m_Context->createShaderModule(fvsmDesc);

    ShaderModuleDesc ffsmDesc;
    ffsmDesc.shaderType = SR_SHADER_FRAGMENT;
    if (TestEnvironment::m_API == SR_API_OPENGL){
        ffsmDesc.code = s_GlslFragmentShaderFragmentSSBOSource;
        ffsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
    }
    auto ffsm = TestEnvironment::m_Context->createShaderModule(ffsmDesc);

    ShaderDesc fshaderDesc;
    fshaderDesc.vertexShader = fvsm;
    fshaderDesc.fragmentShader = ffsm;
    m_FSSBOShader = TestEnvironment::m_Context->createShader(fshaderDesc);

    VertexArrayDesc fvaoDesc;
    fvaoDesc.vertexBuffer = m_VertexBuffer;
    fvaoDesc.vertexShader = fvsm;
    m_FSSBOVertexArray = TestEnvironment::m_Context->createVertexArray(fvaoDesc);

}

void ShaderStorageBufferTest::TearDown() {
    Test::TearDown();
}

TEST_F(ShaderStorageBufferTest, CreateVertexShaderStorageBuffer){
    SSBOPositionData data;
    data.positionData[0] = glm::vec4(0.0f);
    data.positionData[1] = glm::vec4(0.0f);
    data.positionData[2] = glm::vec4(0.0f);

    ShaderStorageBufferDesc desc;
    desc.name = "PositionData";
    desc.size = sizeof(SSBOPositionData);
    desc.data = &data;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;
    auto ssbo = TestEnvironment::m_Context->createShaderStorageBuffer(desc);

    EXPECT_EQ(ssbo->getSize(), sizeof(SSBOPositionData));
    EXPECT_EQ(ssbo->getUsage(), SR_BUFFER_USAGE_DEFAULT);
    EXPECT_EQ(ssbo->getShaderStage(), SR_SHADER_VERTEX);
    EXPECT_EQ(ssbo->getName(), "PositionData");
}

TEST_F(ShaderStorageBufferTest, ReadCPUVertexShaderStorageBuffer){
    SSBOPositionData data;
    data.positionData[0] = glm::vec4(0.0f);
    data.positionData[1] = glm::vec4(0.0f);
    data.positionData[2] = glm::vec4(0.0f);

    ShaderStorageBufferDesc desc;
    desc.name = "PositionData";
    desc.size = sizeof(SSBOPositionData);
    desc.data = &data;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_VERTEX;
    auto ssbo = TestEnvironment::m_Context->createShaderStorageBuffer(desc);

    // fill the SSBO
    TestEnvironment::m_Context->beginRenderPass();
    ssbo->bindShaderResource(0);
    m_VSSBOShader->bind();
    TestEnvironment::m_Context->draw(m_VSSBOVertexArray);

    // read positionData from the filled SSBO
    auto ssboBytes = ssbo->getData();
    auto positionData = *reinterpret_cast<SSBOPositionData*>(ssboBytes.get());

    EXPECT_EQ(positionData.positionData[0].x, s_TriangleVertices[0]);
    EXPECT_EQ(positionData.positionData[0].y, s_TriangleVertices[1]);
    EXPECT_EQ(positionData.positionData[0].z, s_TriangleVertices[2]);

    EXPECT_EQ(positionData.positionData[1].x, s_TriangleVertices[6]);
    EXPECT_EQ(positionData.positionData[1].y, s_TriangleVertices[7]);
    EXPECT_EQ(positionData.positionData[1].z, s_TriangleVertices[8]);

    EXPECT_EQ(positionData.positionData[2].x, s_TriangleVertices[12]);
    EXPECT_EQ(positionData.positionData[2].y, s_TriangleVertices[13]);
    EXPECT_EQ(positionData.positionData[2].z, s_TriangleVertices[14]);
}

TEST_F(ShaderStorageBufferTest, ReadGPUVertexShaderStorageBuffer){
    SSBOPositionData data;
    data.positionData[0] = glm::vec4(0.0f);
    data.positionData[1] = glm::vec4(0.0f);
    data.positionData[2] = glm::vec4(0.0f);
    data.offset.x = 4.0f;
    data.offset.y = 3.0f;
    data.offset.z = 2.0f;

    ShaderStorageBufferDesc desc;
    desc.name = "PositionData";
    desc.size = sizeof(SSBOPositionData);
    desc.data = &data;
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.shaderStage = SR_SHADER_VERTEX;
    auto ssbo = TestEnvironment::m_Context->createShaderStorageBuffer(desc);

    // fill the SSBO
    TestEnvironment::m_Context->beginRenderPass();
    ssbo->bindShaderResource(0);
    m_VSSBOShader->bind();
    TestEnvironment::m_Context->draw(m_VSSBOVertexArray);

    // read positionData from the filled SSBO
    auto ssboBytes = ssbo->getData();
    auto positionData = *reinterpret_cast<SSBOPositionData*>(ssboBytes.get());

    EXPECT_EQ(positionData.positionData[0].x, s_TriangleVertices[0] + 4.0f);
    EXPECT_EQ(positionData.positionData[0].y, s_TriangleVertices[1] + 3.0f);
    EXPECT_EQ(positionData.positionData[0].z, s_TriangleVertices[2] + 2.0f);

    EXPECT_EQ(positionData.positionData[1].x, s_TriangleVertices[6] + 4.0f);
    EXPECT_EQ(positionData.positionData[1].y, s_TriangleVertices[7] + 3.0f);
    EXPECT_EQ(positionData.positionData[1].z, s_TriangleVertices[8] + 2.0f);

    EXPECT_EQ(positionData.positionData[2].x, s_TriangleVertices[12] + 4.0f);
    EXPECT_EQ(positionData.positionData[2].y, s_TriangleVertices[13] + 3.0f);
    EXPECT_EQ(positionData.positionData[2].z, s_TriangleVertices[14] + 2.0f);
}

TEST_F(ShaderStorageBufferTest, ReadCPUReadWriteGPUVertexShaderStorageBuffer){
    SSBOPositionData data;
    data.positionData[0] = glm::vec4(-5.0f, -5.0f, 0.0f, 1.0f);
    data.positionData[1] = glm::vec4(5.0f, -5.0f, 0.0f, 1.0f);
    data.positionData[2] = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
    data.offset.x = 4.0f;
    data.offset.y = 3.0f;
    data.offset.z = 2.0f;
    data.readPositionFromBuffer = glm::bvec4(true);

    ShaderStorageBufferDesc desc;
    desc.name = "PositionData";
    desc.size = sizeof(SSBOPositionData);
    desc.data = &data;
    desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    desc.shaderStage = SR_SHADER_VERTEX;
    auto ssbo = TestEnvironment::m_Context->createShaderStorageBuffer(desc);

    // fill the SSBO
    TestEnvironment::m_Context->beginRenderPass();
    ssbo->bindShaderResource(0);
    m_VSSBOShader->bind();
    TestEnvironment::m_Context->draw(m_VSSBOVertexArray);

    // read positionData from the filled SSBO
    auto ssboBytes = ssbo->getData();
    auto positionData = *reinterpret_cast<SSBOPositionData*>(ssboBytes.get());

    for (uint32 i = 0; i < 3; i++){
        EXPECT_EQ(positionData.positionData[i].x, data.positionData[i].x + 4.0f);
        EXPECT_EQ(positionData.positionData[i].y, data.positionData[i].y + 3.0f);
        EXPECT_EQ(positionData.positionData[i].z, data.positionData[i].z + 2.0f);
    }
}

TEST_F(ShaderStorageBufferTest, CreateFragmentShaderStorageBuffer){
    SSBOColorData data;

    ShaderStorageBufferDesc desc;
    desc.name = "ColorData";
    desc.size = sizeof(SSBOColorData);
    desc.data = &data;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_FRAGMENT;
    auto ssbo = TestEnvironment::m_Context->createShaderStorageBuffer(desc);

    EXPECT_EQ(ssbo->getSize(), sizeof(SSBOColorData));
    EXPECT_EQ(ssbo->getUsage(), SR_BUFFER_USAGE_DEFAULT);
    EXPECT_EQ(ssbo->getShaderStage(), SR_SHADER_FRAGMENT);
    EXPECT_EQ(ssbo->getName(), "ColorData");
}

TEST_F(ShaderStorageBufferTest, ReadCPUFragmentShaderStorageBuffer){
    SSBOColorData data;

    ShaderStorageBufferDesc desc;
    desc.name = "ColorData";
    desc.size = sizeof(SSBOPositionData);
    desc.data = &data;
    desc.usage = SR_BUFFER_USAGE_DEFAULT;
    desc.shaderStage = SR_SHADER_FRAGMENT;
    auto ssbo = TestEnvironment::m_Context->createShaderStorageBuffer(desc);

    // fill the SSBO
    TestEnvironment::m_Context->beginRenderPass();
    ssbo->bindShaderResource(0);
    m_FSSBOShader->bind();
    TestEnvironment::m_Context->draw(m_FSSBOVertexArray);

    // read increment from the filled SSBO
    auto ssboBytes = ssbo->getData();
    auto colorData = *reinterpret_cast<SSBOColorData*>(ssboBytes.get());
    EXPECT_EQ(colorData.increment.x, 60000);

}