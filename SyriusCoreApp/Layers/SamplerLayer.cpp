#include "SamplerLayer.hpp"

SamplerLayer::SamplerLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                           EasyIni::Configuration &config) : Layer(context, window, config) {
    m_ShaderProgram = m_ShaderLibrary.loadShader("Texture");

    Mesh rectangle = createRectangle();
    m_VertexArray = loadMesh(rectangle, m_ShaderProgram);

    glm::mat4 transform = glm::mat4(1.0f);
    ConstantBufferDesc transformDesc;
    transformDesc.data = &transform;
    transformDesc.size = sizeof(glm::mat4);
    transformDesc.shaderStage = SR_SHADER_VERTEX;
    transformDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    transformDesc.name = "TransformData";
    m_TransformBuffer = m_Context->createConstantBuffer(transformDesc);

    ConstantBufferDesc textureParamsDesc;
    textureParamsDesc.data = &m_TextureParameters;
    textureParamsDesc.size = sizeof(TextureParameters);
    textureParamsDesc.shaderStage = SR_SHADER_FRAGMENT;
    textureParamsDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    textureParamsDesc.name = "TextureParameters";
    m_TextureParametersBuffer = m_Context->createConstantBuffer(textureParamsDesc);

    ImageFileDesc img1Desc;
    img1Desc.fileName = "./Resources/Textures/awesomeface.png";
    img1Desc.flipOnAccess = true;
    auto img1 = createImage(img1Desc);
    Texture2DImageDesc tex1Desc;
    tex1Desc.image = createResourceView(img1);
    m_Texture1 = m_Context->createTexture2D(tex1Desc);

    ImageFileDesc img2Desc;
    img2Desc.fileName = "./Resources/Textures/insta.png";
    img2Desc.flipOnAccess = true;
    auto img2 = createImage(img2Desc);
    Texture2DImageDesc tex2Desc;
    tex2Desc.image = createResourceView(img2);
    m_Texture2 = m_Context->createTexture2D(tex2Desc);

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    addImGuiDrawFunction([this]{
        Layer::imGuiRenderTransformConstantBuffer(m_TransformBuffer);
        imGuiRenderTextureParameters(m_TextureParametersBuffer);
    });
}

SamplerLayer::~SamplerLayer() = default;

void SamplerLayer::onUpdate() {
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Texture1->bindShaderResource(0);
    m_Texture2->bindShaderResource(1);
    m_Sampler->bindShaderResource(0);
    m_TransformBuffer->bind(0);
    m_TextureParametersBuffer->bind(4);
    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();
}

void SamplerLayer::onEvent(const Event &event) {

}

void SamplerLayer::imGuiRenderTextureParameters(ResourceView<ConstantBuffer> &constantBuffer) {
    ImGui::Begin("Texture Parameters");
    if (ImGui::SliderFloat("Lerp Factor", &m_TextureParameters.lerpFactor, 0.0f, 1.0f)){
        constantBuffer->setData(&m_TextureParameters, sizeof(TextureParameters));
    }
    if (ImGui::SliderFloat("Scale", &m_TextureParameters.scale, 0.0f, 20.0f)){
        constantBuffer->setData(&m_TextureParameters, sizeof(TextureParameters));
    }
    ImGui::End();
}
