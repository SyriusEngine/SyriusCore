#include "TestCubemap.hpp"
#include <chrono>

float skyboxVertices[] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

TestCubemap::TestCubemap(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config),
m_TestShaderLibrary("./Resources/TestShaders", context),
m_DeltaTime(0.0),
m_LastFrameTime(0.0),
m_Lerp(0.5f),
m_Translation(0.0f),
m_Camera(0.1f, 0.01f, context),
m_Projection(context, window->getWidth(), window->getHeight()){
    m_Window->createImGuiContext();

    m_Context->getDefaultFrameBuffer()->enableDepthTest(true);

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

    // skybox
    auto skyboxVertexLayout = m_Context->createVertexLayout();
    skyboxVertexLayout->addAttribute("Position", SR_FLOAT32_3);

    VertexBufferDesc skVboDesc;
    skVboDesc.data = skyboxVertices;
    skVboDesc.count = 6 * 6;
    skVboDesc.layout = skyboxVertexLayout;
    skVboDesc.usage = SR_BUFFER_USAGE_STATIC;

    auto skyboxVbo = m_Context->createVertexBuffer(skVboDesc);

    m_CubemapProgram = m_TestShaderLibrary.loadShader("Cubemap", "CubeMap");

    VertexArrayDesc skVaoDesc;
    skVaoDesc.vertexBuffer = skyboxVbo;
    skVaoDesc.vertexShader = m_CubemapProgram.vertexShader;
    m_CubemapVertexArray = m_Context->createVertexArray(skVaoDesc);

    ImageFileDesc backDesc;
    backDesc.fileName = "./Resources/Textures/Skybox1/back.jpg";
    auto back = createImage(backDesc);
    back->extendAlpha();
    ImageFileDesc frontDesc;
    frontDesc.fileName = "./Resources/Textures/Skybox1/front.jpg";
    auto front = createImage(frontDesc);
    front->extendAlpha();
    ImageFileDesc topDesc;
    topDesc.fileName = "./Resources/Textures/Skybox1/top.jpg";
    auto top = createImage(topDesc);
    top->extendAlpha();
    ImageFileDesc bottomDesc;
    bottomDesc.fileName = "./Resources/Textures/Skybox1/bottom.jpg";
    auto bottom = createImage(bottomDesc);
    bottom->extendAlpha();
    ImageFileDesc leftDesc;
    leftDesc.fileName = "./Resources/Textures/Skybox1/left.jpg";
    auto left = createImage(leftDesc);
    left->extendAlpha();
    ImageFileDesc rightDesc;
    rightDesc.fileName = "./Resources/Textures/Skybox1/right.jpg";
    auto right = createImage(rightDesc);
    right->extendAlpha();

    CubemapImageDesc cubemapDesc;
    cubemapDesc.back = createResourceView(back);
    cubemapDesc.front = createResourceView(front);
    cubemapDesc.top = createResourceView(top);
    cubemapDesc.bottom = createResourceView(bottom);
    cubemapDesc.left = createResourceView(left);
    cubemapDesc.right = createResourceView(right);
    m_Cubemap = m_Context->createCubemap(cubemapDesc);

}


TestCubemap::~TestCubemap() {
    m_Window->destroyImGuiContext();
}

void TestCubemap::onUpdate() {
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

void TestCubemap::onEvent(const Event &event) {
    m_Camera.update(event, m_DeltaTime);
    if (event.type == SR_EVENT_WINDOW_RESIZED){
        auto width = event.windowWidth;
        auto height = event.windowHeight;
        m_Projection.onResize(width, height);
        m_Context->onResize(width, height);
    }
}

void TestCubemap::render() {
    m_Sampler->bind(0);
    m_Texture->bind(0);

    m_ModelDataBuffer->bind(2);
    m_Camera.bind(1);
    m_Projection.bind(0);
    m_LerpBuffer->bind(4);
    m_ShaderProgram.shaderProgram->bind();
    m_Context->draw(m_VertexArray);
}

void TestCubemap::renderImGui() {
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
    ImGui::End();

    m_Window->onImGuiEnd();
}

