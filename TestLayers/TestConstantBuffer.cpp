#include "TestConstantBuffer.hpp"

TestConstantBuffer::TestConstantBuffer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                         EasyIni::Configuration &config) : Layer(context, window, config),
                         m_TestShaderLibrary("./Resources/TestShaders", context),
                         m_Lerp(0.5f),
                         m_Translation(0.0f){
    m_ShaderProgram = m_TestShaderLibrary.loadShader("ConstantBuffer");

    Mesh rectangle = createRectangle();
    m_VertexArray = loadMesh(rectangle, m_ShaderProgram);

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, m_Translation);
    ConstantBufferDesc transformDesc;
    transformDesc.data = &transform;
    transformDesc.size = sizeof(glm::mat4);
    transformDesc.shaderStage = SR_SHADER_VERTEX;
    transformDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    transformDesc.name = "TransformData";
    m_TransformBuffer = m_Context->createConstantBuffer(transformDesc);

    auto lerp = glm::vec4(m_Lerp);
    ConstantBufferDesc lerpDesc;
    lerpDesc.data = &lerp;
    lerpDesc.size = sizeof(glm::vec4);
    lerpDesc.shaderStage = SR_SHADER_FRAGMENT;
    lerpDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    lerpDesc.name = "LerpData";
    m_LerpBuffer = m_Context->createConstantBuffer(lerpDesc);

    ImageFileDesc imgDesc;
    imgDesc.fileName = "./Resources/Textures/awesomeface.png";
    imgDesc.flipOnAccess = true;
    auto img = createImage(imgDesc);
    Texture2DImageDesc texDesc(img);
    m_Texture = m_Context->createTexture2D(texDesc);

    m_Window->createImGuiContext();
}

TestConstantBuffer::~TestConstantBuffer() {

}

void TestConstantBuffer::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Texture->bind(0);
    m_Sampler->bind(0);
    m_TransformBuffer->bind(0);
    m_LerpBuffer->bind(1);
    m_Context->draw(m_VertexArray);

    m_Window->onImGuiBegin();
    ImGui::Begin("Constant Buffer");
    if (ImGui::SliderFloat3("Translation", &m_Translation.x, -1.0f, 1.0f)){
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Translation);
        m_TransformBuffer->setData(&transform);
    }
    if (ImGui::SliderFloat("Lerp", &m_Lerp, 0.0f, 1.0f)){
        glm::vec4 lerp = glm::vec4(m_Lerp);
        m_LerpBuffer->setData(&lerp);
    }
    ImGui::End();

    m_Window->onImGuiEnd();

    m_Context->endRenderPass();
}

void TestConstantBuffer::onEvent(const Event &event) {

}
