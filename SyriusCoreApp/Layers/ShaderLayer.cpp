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
    m_Params.mouse = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

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
        shaderSelectorPanel();

    });
}

ShaderLayer::~ShaderLayer() = default;

void ShaderLayer::onUpdate() {
    Layer::onUpdate();

    auto currentTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());

    m_Params.deltaTime = m_DeltaTime;
    if (m_UseTime){
        m_Params.time = static_cast<float>((currentTime - m_StartTime) / 1000.0);
    }
    else{
        m_Params.time = t;
    }
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
        case SR_EVENT_MOUSE_MOVED:
            m_Params.mouse.x = event.mousePosX;
            m_Params.mouse.y = event.mousePosY;
            break;
        case SR_EVENT_MOUSE_BUTTON_PRESSED:
            m_Params.mouse.z = 1.0f;
            break;
        case SR_EVENT_MOUSE_BUTTON_RELEASED:
            m_Params.mouse.z = 0.0f;
            break;
        default:
            break;

    }

}

void ShaderLayer::shaderSelectorPanel() {
    imGuiBeginPanel("Shader Selector");

    static std::vector<std::string> shaders = {
            "FractalPyramid",
            "Mandelbulb",
            "Octagrams",
            "PulsatingWaves",
            "Apollonian",
            "LightAndMotion",
    };
    static int selectedShader = 0;
    if (ImGui::BeginCombo("Shaders", shaders[selectedShader].c_str())) {
        for (int i = 0; i < shaders.size(); i++) {
            bool isSelected = selectedShader == i;
            if (ImGui::Selectable(shaders[i].c_str(), isSelected)) {
                selectedShader = i;
                auto vsShader = m_Config["ShaderLayer"]["VertexShader"].get<std::string>();
                m_ShaderProgram = m_ShaderLibrary.loadShader(vsShader, shaders[i]);
                printf("Recompiling shader %s\n", shaders[i].c_str());
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Use Time", &m_UseTime);
    if (!m_UseTime){
        ImGui::SliderFloat("Time", &t, 0.0f, 20.0f);
    }

    imGuiEndPanel();
}
