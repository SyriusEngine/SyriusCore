#include "TextureAtlasLayer.hpp"

TextureAtlasLayer::TextureAtlasLayer(ResourceView<Context> &context, const UP<SyriusWindow> &window,
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
    TextureParameters params;
    textureParamsDesc.data = &params;
    textureParamsDesc.size = sizeof(TextureParameters);
    textureParamsDesc.shaderStage = SR_SHADER_FRAGMENT;
    textureParamsDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    textureParamsDesc.name = "TextureParameters";
    m_TextureParametersBuffer = m_Context->createConstantBuffer(textureParamsDesc);

    ImageFileDesc img1Desc;
    img1Desc.fileName = "./Resources/Textures/awesomeface512.png";
    img1Desc.flipOnAccess = true;
    auto img1 = createImage(img1Desc);
    ImageFileDesc img2Desc;
    img2Desc.fileName = "./Resources/Textures/insta512.png";
    img2Desc.flipOnAccess = true;
    auto img2 = createImage(img2Desc);

    // Create a texture with a specific size
    Texture2DDesc tex1Desc;
    tex1Desc.width = 1024;
    tex1Desc.height = 512;
    tex1Desc.format = SR_TEXTURE_RGBA_UI8;
    tex1Desc.data = nullptr;
    m_Texture1 = m_Context->createTexture2D(tex1Desc);

    // now we can update the texture with the image data
    m_Texture1->setData(img1->getData(), 0, 0, img1->getWidth(), img1->getHeight());
    m_Texture1->setData(img2->getData(), 512, 0, img2->getWidth(), img2->getHeight());

    Texture2DImageDesc tex2Desc;
    tex2Desc.image = createResourceView(img2);
    tex2Desc.usage = SR_BUFFER_USAGE_STATIC;
    m_Texture2 = m_Context->createTexture2D(tex2Desc);

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    addImGuiDrawFunction([this]{
        Layer::imGuiRenderTransformConstantBuffer(m_TransformBuffer);
        Layer::imGuiTextureParametersPanel(m_TextureParametersBuffer);
        Layer::imGuiSamplerPanel(m_Sampler);
        Layer::imGuiTexturePanel(m_Texture1);
    });
}

TextureAtlasLayer::~TextureAtlasLayer() = default;

void TextureAtlasLayer::onUpdate() {
    Layer::onUpdate();

    m_Context->clear();
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Texture1->bindShaderResource(0);
    m_Texture2->bindShaderResource(1);
    m_Sampler->bindShaderResource(0);
    m_TransformBuffer->bindShaderResource(0);
    m_TextureParametersBuffer->bindShaderResource(4);
    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();
}

void TextureAtlasLayer::onEvent(const Event &event) {

}
