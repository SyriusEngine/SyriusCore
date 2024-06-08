#include "FrameBufferLayer.hpp"

FrameBufferLayer::FrameBufferLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config),
m_Camera(0.1f, 0.01f, context),
m_SelectedTexture(0),
m_Projection(context, window->getWidth(), window->getHeight()){
    m_ShaderProgram = m_ShaderLibrary.loadShader("MVP", "FrameBuffer");

    Mesh cube = createCube();
    m_VertexArray = loadMesh(cube, m_ShaderProgram);

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
    img1Desc.fileName = "./Resources/Textures/awesomeface.png";
    img1Desc.flipOnAccess = true;
    img1Desc.requestedChannelCount = 4;
    auto img1 = createImage(img1Desc);
    Texture2DImageDesc tex1Desc;
    tex1Desc.usage = SR_BUFFER_USAGE_DEFAULT;
    tex1Desc.image = createResourceView(img1);
    m_Texture1 = m_Context->createTexture2D(tex1Desc);

    ImageFileDesc img2Desc;
    img2Desc.fileName = "./Resources/Textures/insta512.png";
    img2Desc.flipOnAccess = true;
    auto img2 = createImage(img2Desc);
    Texture2DImageDesc tex2Desc;
    tex2Desc.image = createResourceView(img2);
    tex2Desc.usage = SR_BUFFER_USAGE_DEFAULT;
    m_Texture2 = m_Context->createTexture2D(tex2Desc);

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    // Framebuffer stuff
    auto fbLayout = m_Context->createFrameBufferLayout();
    ViewportDesc vpDesc;
    vpDesc.width = m_Context->getWidth();
    vpDesc.height = m_Context->getHeight();
    fbLayout->addViewportDesc(vpDesc);
    ColorAttachmentDesc f32ca;
    f32ca.format = SR_TEXTURE_RGBA_F32;
    f32ca.width = m_Context->getWidth();
    f32ca.height = m_Context->getHeight();
    f32ca.clearColor[0] = 1.0f;
    fbLayout->addColorAttachmentDesc(f32ca); // first color attachment
    fbLayout->addColorAttachmentDesc(f32ca); // second color attachment
    DepthStencilAttachmentDesc dsDesc;
    dsDesc.format = SR_TEXTURE_DEPTH_24_STENCIL_8;
    dsDesc.width = m_Context->getWidth();
    dsDesc.height = m_Context->getHeight();
    //dsDesc.enableShaderAccess = true;
    fbLayout->addDepthStencilAttachmentDesc(dsDesc);

    m_FrameBuffer = m_Context->createFrameBuffer(fbLayout);

    m_ScreenProgram = m_ShaderLibrary.loadShader("Screen");

    Mesh rectangle = createRectangle(0.75f);
    m_ScreenVertexArray = loadMesh(rectangle, m_ScreenProgram);

    addImGuiDrawFunction([this]{
        Layer::imGuiRenderTransformConstantBuffer(m_TransformBuffer);
        Layer::imGuiTextureParametersPanel(m_TextureParametersBuffer);
        Layer::imGuiSamplerPanel(m_Sampler);
        Layer::imGuiTexturePanel(m_Texture1);
        auto defaultFb = m_Context->getDefaultFrameBuffer();
        Layer::imGuiDepthTestPanel(defaultFb);
        Layer::imGuiCameraPanel(m_Camera);
        Layer::imGuiFrameBufferPanel(m_FrameBuffer, m_SelectedTexture);
    });
}

FrameBufferLayer::~FrameBufferLayer() = default;

void FrameBufferLayer::onUpdate() {
    Layer::onUpdate();

    // render pass 1
    m_Context->beginRenderPass(m_FrameBuffer);

    m_ShaderProgram.shaderProgram->bind();
    m_Texture1->bindShaderResource(0);
    m_Texture2->bindShaderResource(1);
    m_Sampler->bindShaderResource(0);
    m_TransformBuffer->bind(0);
    m_Camera.bind(1);
    m_Projection.bind(2);
    m_TextureParametersBuffer->bind(4);
    m_Context->draw(m_VertexArray);

    m_Context->endRenderPass(m_FrameBuffer);

    // render pass 2
    m_Context->beginRenderPass();
    switch (m_SelectedTexture) {
        case 0: m_FrameBuffer->getColorAttachment(0)->bindShaderResource(0); break;
        case 1: m_FrameBuffer->getColorAttachment(1)->bindShaderResource(0); break;
        case 2: m_FrameBuffer->getDepthStencilAttachment()->bindShaderResource(0); break;
        default:
            break;
    }
    m_ScreenProgram.shaderProgram->bind();
    m_Context->draw(m_ScreenVertexArray);

    renderImGui();

    m_Context->endRenderPass();
}

void FrameBufferLayer::onEvent(const Event &event) {
    m_Camera.update(event, m_DeltaTime);
}
