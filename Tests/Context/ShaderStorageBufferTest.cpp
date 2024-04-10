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

    ShaderModuleDesc vsmDesc;
    vsmDesc.shaderType = SR_SHADER_VERTEX;
    if (TestEnvironment::m_API == SR_API_OPENGL){
        vsmDesc.code = s_GLSLSSBOVertexShaderSource;
        vsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
    }
    auto vsm = TestEnvironment::m_Context->createShaderModule(vsmDesc);

    ShaderModuleDesc fsmDesc;
    fsmDesc.shaderType = SR_SHADER_FRAGMENT;
    if (TestEnvironment::m_API == SR_API_OPENGL){
        fsmDesc.code = s_GLSLSSBOFragmentShaderSource;
        fsmDesc.language = SR_SHADER_LANGUAGE_GLSL;
    }
    auto fsm = TestEnvironment::m_Context->createShaderModule(fsmDesc);

    ShaderDesc shaderDesc;
    shaderDesc.vertexShader = vsm;
    shaderDesc.fragmentShader = fsm;
    m_Shader = TestEnvironment::m_Context->createShader(shaderDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.vertexBuffer = m_VertexBuffer;
    vaoDesc.vertexShader = vsm;
    m_VertexArray = TestEnvironment::m_Context->createVertexArray(vaoDesc);
}

void ShaderStorageBufferTest::TearDown() {
    Test::TearDown();
}

TEST_F(ShaderStorageBufferTest, CreateShaderStorageBuffer){
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

TEST_F(ShaderStorageBufferTest, ReadCPUShaderStorageBuffer){
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
    m_Shader->bind();
    TestEnvironment::m_Context->draw(m_VertexArray);

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

TEST_F(ShaderStorageBufferTest, ReadGPUShaderStorageBuffer){
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
    m_Shader->bind();
    TestEnvironment::m_Context->draw(m_VertexArray);

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

TEST_F(ShaderStorageBufferTest, ReadCPUReadWriteGPUShaderStorageBufferTest){
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
    m_Shader->bind();
    TestEnvironment::m_Context->draw(m_VertexArray);

    // read positionData from the filled SSBO
    auto ssboBytes = ssbo->getData();
    auto positionData = *reinterpret_cast<SSBOPositionData*>(ssboBytes.get());

    for (uint32 i = 0; i < 3; i++){
        EXPECT_EQ(positionData.positionData[i].x, data.positionData[i].x + 4.0f);
        EXPECT_EQ(positionData.positionData[i].y, data.positionData[i].y + 3.0f);
        EXPECT_EQ(positionData.positionData[i].z, data.positionData[i].z + 2.0f);
    }
}
