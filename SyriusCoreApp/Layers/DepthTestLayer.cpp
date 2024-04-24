#include "DepthTestLayer.hpp"

DepthTestLayer::DepthTestLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                               EasyIni::Configuration &config) :
Layer(context, window, config),
m_DeltaTime(0.0f), m_LastFrameTime(0.0f),
m_Camera(0.1f, 0.01f, context),
m_Projection(context, window->getWidth(), window->getHeight()){
    m_ShaderProgram = m_ShaderLibrary.loadShader("MVP", "Texture");

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
    img1Desc.fileName = "./Resources/Textures/awesomeface512.png";
    img1Desc.flipOnAccess = true;
    auto img1 = createImage(img1Desc);
    Texture2DImageDesc tex1Desc;
    tex1Desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    tex1Desc.image = createResourceView(img1);
    m_Texture1 = m_Context->createTexture2D(tex1Desc);

    ImageFileDesc img2Desc;
    img2Desc.fileName = "./Resources/Textures/insta512.png";
    img2Desc.flipOnAccess = true;
    auto img2 = createImage(img2Desc);
    Texture2DImageDesc tex2Desc;
    tex2Desc.image = createResourceView(img2);
    tex2Desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    m_Texture2 = m_Context->createTexture2D(tex2Desc);

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    addImGuiDrawFunction([this]{
        Layer::imGuiRenderTransformConstantBuffer(m_TransformBuffer);
        Layer::imGuiTextureParametersPanel(m_TextureParametersBuffer);
        Layer::imGuiSamplerPanel(m_Sampler);
        Layer::imGuiTexturePanel(m_Texture1);
        auto defaultFb = m_Context->getDefaultFrameBuffer();
        Layer::imGuiDepthTestPanel(defaultFb);
        Layer::imGuiCameraPanel(m_Camera);
    });
}

DepthTestLayer::~DepthTestLayer() = default;

void DepthTestLayer::onUpdate() {
    auto currentTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
    m_DeltaTime = currentTime - m_LastFrameTime;
    m_LastFrameTime = currentTime;

    // render
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_Texture1->bindShaderResource(0);
    m_Texture2->bindShaderResource(1);
    m_Sampler->bindShaderResource(0);
    m_TransformBuffer->bind(0);
    m_Camera.bind(1);
    m_Projection.bind(2);
    m_TextureParametersBuffer->bind(4);
    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();

}

void DepthTestLayer::onEvent(const Event &event) {
    m_Camera.update(event, m_DeltaTime);
    switch (event.type) {
        case SR_EVENT_WINDOW_RESIZED: {
            m_Context->onResize(event.windowWidth, event.windowHeight);
            break;
        }
        default:
            break;
    }
}
