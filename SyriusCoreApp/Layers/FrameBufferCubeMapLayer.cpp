#include "FrameBufferCubeMapLayer.hpp"

FrameBufferCubeMapLayer::FrameBufferCubeMapLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config),
m_Camera(0.1f, 0.01f, context),
m_Projection(context, window->getWidth(), window->getHeight()){
    glm::mat4 transform = glm::mat4(1.0f);
    ConstantBufferDesc transformDesc;
    transformDesc.data = &transform;
    transformDesc.size = sizeof(glm::mat4);
    transformDesc.shaderStage = SR_SHADER_VERTEX;
    transformDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    transformDesc.name = "TransformData";
    m_TransformBuffer = m_Context->createConstantBuffer(transformDesc);

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    m_ShaderProgram = m_ShaderLibrary.loadShader("MVP");
    auto cube = createCube();
    m_VertexArray = loadMesh(cube, m_ShaderProgram);

    ImageFileDesc img1Desc;
    // img1Desc.fileName = "./Resources/Textures/HDR/photo_studio_loft_hall_2k.hdr";
    img1Desc.fileName = "./Resources/Textures/HDR/citrus_orchard_road_1k.hdr";
    img1Desc.flipOnAccess = true;
    auto img1 = createImage(img1Desc);

    Texture2DImageDesc tex1Desc;
    tex1Desc.usage = SR_BUFFER_USAGE_DYNAMIC;
    tex1Desc.image = createResourceView(img1);
    m_Texture1 = m_Context->createTexture2D(tex1Desc);

    ImageFileDesc wFile;
    wFile.fileName = "./Resources/Textures/out.hdr";
    wFile.flipOnAccess = true;
    wFile.imgType = SR_IMAGE_HDR;
    img1->writeToFile(wFile);

    addImGuiDrawFunction([this]{
        Layer::imGuiRenderTransformConstantBuffer(m_TransformBuffer);
        Layer::imGuiSamplerPanel(m_Sampler);
        auto defaultFb = m_Context->getDefaultFrameBuffer();
        Layer::imGuiDepthTestPanel(defaultFb);
        Layer::imGuiCameraPanel(m_Camera);
    });

}

FrameBufferCubeMapLayer::~FrameBufferCubeMapLayer() = default;

void FrameBufferCubeMapLayer::onUpdate() {
    Layer::onUpdate();

    m_Context->beginRenderPass();
    m_ShaderProgram.shaderProgram->bind();
    m_TransformBuffer->bind(0);
    m_Camera.bind(1);
    m_Projection.bind(2);
    m_Sampler->bindShaderResource(0);
    m_Texture1->bindShaderResource(0);

    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();
}

void FrameBufferCubeMapLayer::onEvent(const Event &event) {
    Layer::onEvent(event);

    m_Camera.update(event, m_DeltaTime);
}
