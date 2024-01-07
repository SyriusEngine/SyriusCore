#include "TestDepthTest.hpp"
#include <chrono>

TestDepthTest::TestDepthTest(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config),
m_TestShaderLibrary("./Resources/TestShaders", context),
m_DeltaTime(0.0),
m_LastFrameTime(0.0),
m_Lerp(0.5f),
m_Translation(0.0f),
m_DepthTest(false),
m_DepthMask(true),
m_Camera(0.1f, 0.02f, context),
m_Projection(context, window->getWidth(), window->getHeight()){
    m_Window->createImGuiContext();

    m_ShaderProgram = m_TestShaderLibrary.loadShader("MVP", "ConstantBuffer");

    Mesh cube = createCube();
    m_VertexArray = loadMesh(cube, m_ShaderProgram);

    SamplerDesc splrDesc;
    m_Sampler = m_Context->createSampler(splrDesc);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, m_Translation);
    ConstantBufferDesc transformDesc;
    transformDesc.data = &transform;
    transformDesc.size = sizeof(glm::mat4);
    transformDesc.shaderStage = SR_SHADER_VERTEX;
    transformDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    transformDesc.name = "ModelData";
    m_ModelDataBuffer = m_Context->createConstantBuffer(transformDesc);

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
}

TestDepthTest::~TestDepthTest() {
    m_Window->destroyImGuiContext();
}

void TestDepthTest::onUpdate() {
    auto currentTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
    m_DeltaTime = currentTime - m_LastFrameTime;
    m_LastFrameTime = currentTime;

    m_Context->beginRenderPass();
    render();
    renderImGui();
    m_Context->endRenderPass();

}

void TestDepthTest::onEvent(const Event &event) {
    m_Camera.update(event, m_DeltaTime);
    if (event.type == SR_EVENT_WINDOW_RESIZED){
        auto width = event.windowWidth;
        auto height = event.windowHeight;
        m_Projection.onResize(width, height);
        m_Context->onResize(width, height);
    }
}

void TestDepthTest::render() {
    m_Sampler->bind(0);
    m_Texture->bind(0);

    m_ModelDataBuffer->bind(2);
    m_Camera.bind(1);
    m_Projection.bind(0);
    m_LerpBuffer->bind(4);
    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);
}

void TestDepthTest::renderImGui() {
    m_Window->onImGuiBegin();

    ImGui::Begin("Debug");
    if (m_Camera.isEnabled()){
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Camera enabled");
    }
    else{
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Camera disabled");
    }
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    if (ImGui::DragFloat3("Translation", &m_Translation.x, 0.1f)){
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Translation);
        m_ModelDataBuffer->setData(&transform);
    }
    if (ImGui::SliderFloat("Lerp", &m_Lerp, 0.0f, 1.0f)){
        auto lerp = glm::vec4(m_Lerp);
        m_LerpBuffer->setData(&lerp);
    }
    if (ImGui::Checkbox("Depth test", &m_DepthTest)){
        m_Context->getDefaultFrameBuffer()->enableDepthTest(m_DepthTest);
    }
    if (ImGui::Checkbox("Depth Mash", &m_DepthMask)){
        m_Context->getDefaultFrameBuffer()->setDepthMask(static_cast<SR_DEPTH_MASK>(m_DepthMask));
    }
    ImGui::End();

    m_Window->onImGuiEnd();
}
