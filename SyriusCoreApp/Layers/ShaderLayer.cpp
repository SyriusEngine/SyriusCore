#include <chrono>
#include "ShaderLayer.hpp"

ShaderLayer::ShaderLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window,
                         EasyIni::Configuration &config) : Layer(context, window, config), m_Params(),
                         m_StartTime(static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count())) {
    m_Params.resolution = glm::vec2(window->getWidth(), window->getHeight());
    m_Params.time = 0.0f;
    m_Params.deltaTime = 0.0f;

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    Mesh rectangle = createScreenRectangle();
    m_VertexArray = loadMesh(rectangle, m_ShaderProgram);

    auto vsShader = config["ShaderLayer"]["VertexShader"].get<std::string>();
    auto fsShader = config["ShaderLayer"]["FragmentShader"].get<std::string>();
    m_ShaderProgram = m_ShaderLibrary.loadShader(vsShader, fsShader);

    ConstantBufferDesc paramDesc;
    paramDesc.data = &m_Params;
    paramDesc.size = sizeof(Parameters);
    paramDesc.shaderStage = SR_SHADER_FRAGMENT;
    paramDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    paramDesc.name = "Parameters";
    m_ParameterBuffer = m_Context->createConstantBuffer(paramDesc);

    addImGuiDrawFunction([this]{

    });
}

ShaderLayer::~ShaderLayer() {

}

void ShaderLayer::onUpdate() {
    auto currentTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
    m_DeltaTime = currentTime - m_LastFrameTime;
    m_LastFrameTime = currentTime;

    m_Params.deltaTime = m_DeltaTime;
    m_Params.time = static_cast<float>((currentTime - m_StartTime) / 1000.0);
    m_ParameterBuffer->setData(&m_Params, sizeof(Parameters));

    m_Context->beginRenderPass();

    m_ShaderProgram.shaderProgram->bind();
    //m_Texture1->bindShaderResource(0);
    m_Sampler->bindShaderResource(0);
    m_ParameterBuffer->bind(5);
    m_Context->draw(m_VertexArray);

    renderImGui();

    m_Context->endRenderPass();
}

void ShaderLayer::onEvent(const Event &event) {
    switch (event.type) {
        case SR_EVENT_WINDOW_RESIZED:
            m_Context->onResize(event.windowWidth, event.windowHeight);
            m_Params.resolution = glm::vec2(event.windowWidth, event.windowHeight);
            break;
        default:
            break;

    }

}
