#include "DeferredPBRLayer.hpp"

static bool depthTest = true;

DeferredPBRLayer::DeferredPBRLayer(ResourceView<Context>& context, const UP<SyriusWindow>& window, EasyIni::Configuration& config) :
Layer(context, window, config),
m_Material(context, "chipped-paint-metal"),
m_Camera(0.1f, 0.01f, context),
m_Projection(context, window->getWidth(), window->getHeight()){
    m_GeometryPass = m_ShaderLibrary.loadShader("GeometryPass");

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    //Mesh cube = createCube();
    //m_Model = loadMesh(cube, m_GeometryPass);

    Mesh sphere = createSphere(32, 32);
    m_Model = loadMesh(sphere, m_GeometryPass);

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

    // Prepare G-Buffer
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
    fbLayout->addColorAttachmentDesc(f32ca); // Position
    fbLayout->addColorAttachmentDesc(f32ca); // Normal
    fbLayout->addColorAttachmentDesc(f32ca); // Albedo
    fbLayout->addColorAttachmentDesc(f32ca); // mrao
    DepthStencilAttachmentDesc dsDesc;
    dsDesc.format = SR_TEXTURE_DEPTH_24_STENCIL_8;
    dsDesc.width = m_Context->getWidth();
    dsDesc.height = m_Context->getHeight();
    dsDesc.clearDepth = 1.0f;
    dsDesc.enableDepthTest = depthTest;
    fbLayout->addDepthStencilAttachmentDesc(dsDesc);
    m_GBuffer = m_Context->createFrameBuffer(fbLayout);

    m_LightPass = m_ShaderLibrary.loadShader("LightPass");
    Mesh rectangle = createRectangle(1.0f);
    m_ScreenRectangle = loadMesh(rectangle, m_LightPass);

    // By default, create 1 light
    m_LightData.positions[0] = glm::vec4(0.0f, 3.0f, 0.0f, 1.0f);
    m_LightData.colors[0] = glm::vec4(255.0f, 255.0f, 255.0f, 1.0f);
    m_LightData.count = glm::uvec4(1, 0, 0, 0);
    ConstantBufferDesc lightDesc;
    lightDesc.data = &m_LightData;
    lightDesc.size = sizeof(LightData);
    lightDesc.shaderStage = SR_SHADER_FRAGMENT;
    lightDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    lightDesc.name = "LightData";
    m_LightBuffer = m_Context->createConstantBuffer(lightDesc);

    addImGuiDrawFunction([this]{
        Layer::imGuiRenderTransformConstantBuffer(m_TransformBuffer);
        Layer::imGuiTextureParametersPanel(m_TextureParametersBuffer);
        Layer::imGuiSamplerPanel(m_Sampler);
        auto defaultFb = m_Context->getDefaultFrameBuffer();
        Layer::imGuiDepthTestPanel(defaultFb);
        Layer::imGuiCameraPanel(m_Camera);
        imGuiGBufferPanel();
    });
}

DeferredPBRLayer::~DeferredPBRLayer() = default;

void DeferredPBRLayer::onUpdate() {
    Layer::onUpdate();

    m_Sampler->bindShaderResource(0);

    // Pass 1: Geometry Pass which fills the G-Buffer
    m_GBuffer->clear();
    m_Context->beginRenderPass(m_GBuffer);

    m_GeometryPass.shaderProgram->bind();
    m_Material.bind(0);
    m_TransformBuffer->bindShaderResource(0);
    m_Camera.bind(1);
    m_Projection.bind(2);
    m_TextureParametersBuffer->bindShaderResource(4);
    m_Context->draw(m_Model);

    m_Context->endRenderPass(m_GBuffer);

    // Pass 2: Lighting Pass
    m_Context->clear();
    m_Context->beginRenderPass();

    m_LightPass.shaderProgram->bind();
    m_LightBuffer->bindShaderResource(5);
    m_GBuffer->getColorAttachment(0)->bindShaderResource(0);
    m_GBuffer->getColorAttachment(1)->bindShaderResource(1);
    m_GBuffer->getColorAttachment(2)->bindShaderResource(2);
    m_GBuffer->getColorAttachment(3)->bindShaderResource(3);
    m_Context->draw(m_ScreenRectangle);

    m_Context->endRenderPass();

   renderImGui();
}

void DeferredPBRLayer::onEvent(const Event &event) {
    m_Camera.update(event, m_DeltaTime);
    m_Projection.onEvent(event);
    if (event.type == SR_EVENT_WINDOW_RESIZED) {
        m_GBuffer->onResize(event.windowWidth, event.windowHeight);
    }
}

void DeferredPBRLayer::imGuiGBufferPanel() {
    imGuiBeginPanel("GBuffer");
    if (ImGui::Checkbox("Depth Test", &depthTest)){
        m_GBuffer->enableDepthTest(depthTest);
    }
    auto positionClearColor = m_GBuffer->getColorAttachment(0)->getClearColor();
    if (ImGui::ColorPicker3("Positions Color", positionClearColor)){
        m_GBuffer->getColorAttachment(0)->setClearColor(positionClearColor[0], positionClearColor[1], positionClearColor[2], 1.0f);
    }


    imGuiEndPanel();
}

