#include "CubeMapLayer.hpp"

CubeMapLayer::CubeMapLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config),
m_Camera(0.1f, 0.01f, context),
m_Projection(context, window->getWidth(), window->getHeight()){
    m_ShaderProgram = m_ShaderLibrary.loadShader("Cubemap");

    Mesh cube = createBigCube();
    m_VertexArray = loadMesh(cube, m_ShaderProgram);

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

    auto cubeMapLayout = m_Context->createCubeMapLayout(2048, 2048, SR_TEXTURE_RGBA_UI8);
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_BACK, "./Resources/Textures/Skybox1/back.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_FRONT, "./Resources/Textures/Skybox1/front.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_LEFT, "./Resources/Textures/Skybox1/left.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_RIGHT, "./Resources/Textures/Skybox1/right.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_TOP, "./Resources/Textures/Skybox1/top.jpg");
    cubeMapLayout->addFaceFromFile(SR_CUBEMAP_FACE_BOTTOM, "./Resources/Textures/Skybox1/bottom.jpg");

    m_CubeMap = m_Context->createCubeMap(cubeMapLayout);

    addImGuiDrawFunction([this]{
        Layer::imGuiRenderTransformConstantBuffer(m_TransformBuffer);
        Layer::imGuiSamplerPanel(m_Sampler);
        auto defaultFb = m_Context->getDefaultFrameBuffer();
        Layer::imGuiDepthTestPanel(defaultFb);
        Layer::imGuiCameraPanel(m_Camera);
    });
}

CubeMapLayer::~CubeMapLayer() = default;

void CubeMapLayer::onUpdate() {
    Layer::onUpdate();

    // render
    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    m_TransformBuffer->bind(0);
    m_Camera.bind(1);
    m_Projection.bind(2);
    m_Sampler->bindShaderResource(0);
    m_CubeMap->bindShaderResource(0);
    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();
}

void CubeMapLayer::onEvent(const Event &event) {
    m_Camera.update(event, m_DeltaTime);
}
