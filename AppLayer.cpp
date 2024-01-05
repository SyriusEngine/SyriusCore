#include <chrono>
#include "AppLayer.hpp"

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

AppLayer::AppLayer(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config):
Layer(context, window, config),
m_DeltaTime(0.0),
m_LastFrameTime(0.0),
m_Camera(0.1f, 0.02f, context),
m_Projection(context, window->getWidth(), window->getHeight()){
    m_Window->createImGuiContext();

    auto mesh = createCube();
    m_ShaderProgram = m_ShaderLibrary.loadShader("Geometry", "Basic");
    m_VertexArray = loadMesh(mesh, m_ShaderProgram);

    SamplerDesc samplerDesc;
    m_Sampler = m_Context->createSampler(samplerDesc);
    ImageFileDesc imgDesc;
    imgDesc.fileName = "./Resources/Textures/awesomeface.png";
    auto image = createImage(imgDesc);
    Texture2DImageDesc t2dDesc(image);
    m_Texture = m_Context->createTexture2D(t2dDesc);

    glm::mat4 model = glm::mat4(1.0f);
    ConstantBufferDesc cbDesc;
    cbDesc.size = sizeof(glm::mat4);
    cbDesc.name = "ModelData";
    cbDesc.data = &model;
    m_ModelDataBuffer = m_Context->createConstantBuffer(cbDesc);



    /// SKYBOX

    ImageFileDesc right;
    right.fileName = "./Resources/Textures/Skybox1/right.jpg";
    ImageFileDesc left;
    left.fileName = "./Resources/Textures/Skybox1/left.jpg";
    ImageFileDesc top;
    top.fileName = "./Resources/Textures/Skybox1/top.jpg";
    ImageFileDesc bottom;
    bottom.fileName = "./Resources/Textures/Skybox1/bottom.jpg";
    ImageFileDesc front;
    front.fileName = "./Resources/Textures/Skybox1/front.jpg";
    ImageFileDesc back;
    back.fileName = "./Resources/Textures/Skybox1/back.jpg";
    CubemapImageDesc cmiDesc;
    auto rightImg = createImage(right);
    auto leftImg = createImage(left);
    auto topImg = createImage(top);
    auto bottomImg = createImage(bottom);
    auto frontImg = createImage(front);
    auto backImg = createImage(back);
    cmiDesc.right = rightImg.createView();
    cmiDesc.left = leftImg.createView();
    cmiDesc.top = topImg.createView();
    cmiDesc.bottom = bottomImg.createView();
    cmiDesc.front = frontImg.createView();
    cmiDesc.back = backImg.createView();
    m_Cubemap = m_Context->createCubemap(cmiDesc);

    m_SbShader = m_ShaderLibrary.loadShader("Skybox", "Skybox");

    auto skyboxLayout = m_Context->createVertexDescription();
    skyboxLayout->addAttribute("Position", SR_FLOAT32_3);

    VertexBufferDesc sbVboDesc;
    sbVboDesc.data = skyboxVertices;
    sbVboDesc.count = 36;
    sbVboDesc.layout = skyboxLayout;
    auto sbVbo = m_Context->createVertexBuffer(sbVboDesc);

    VertexArrayDesc sbVaoDesc;
    sbVaoDesc.vertexBuffer = sbVbo;
    sbVaoDesc.vertexShader = m_SbShader.vertexShader;
    m_SbVAO = m_Context->createVertexArray(sbVaoDesc);
}

AppLayer::~AppLayer() {
    m_Window->destroyImGuiContext();
}

void AppLayer::onUpdate() {
    updateTime();
    render();
    renderImGui();
}

void AppLayer::onEvent(const Event &event) {
    m_Camera.update(event, m_DeltaTime);

}

void AppLayer::updateTime() {
    auto currentTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
    m_DeltaTime = currentTime - m_LastFrameTime;
    m_LastFrameTime = currentTime;

}

void AppLayer::render() {
    m_Context->beginRenderPass();

    m_Sampler->bind(0);
    m_Camera.bind();
    m_Projection.bind();

    m_SbShader.shaderProgram->bind();
    m_Cubemap->bind(0);
    m_Context->draw(m_SbVAO);

//    m_Texture->bind(0);
//    m_ShaderProgram.shaderProgram->bind();
//    m_ModelDataBuffer->bind(2);
//    m_Context->draw(m_VertexArray);

    m_Context->endRenderPass();
}

void AppLayer::renderImGui() {
    m_Window->onImGuiBegin();

    ImGui::Begin("SyriusCoreDev");
    ImGui::Text("FPS: %.2f", 1000.0 / m_DeltaTime);
    ImGui::ColorPicker3("Background Color", m_Context->getDefaultFrameBuffer()->getColorAttachment(0)->getClearColor());

    ImGui::End();

    m_Window->onImGuiEnd();
}
