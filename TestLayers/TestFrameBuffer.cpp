#include "TestFrameBuffer.hpp"
#include <chrono>

TestFrameBuffer::TestFrameBuffer(ResourceView<Context> &context, const Resource<SyriusWindow> &window, EasyIni::Configuration &config) :
Layer(context, window, config),
m_TestShaderLibrary("./Resources/TestShaders", context),
m_DeltaTime(0.0),
m_LastFrameTime(0.0),
m_Lerp(0.5f),
m_TextureResample(1.0f),
m_Translation(0.0f),
m_DepthTest(true),
m_DepthMask(true),
m_SampleFromColorAttachment(true),
m_Camera(0.1f, 0.02f, context),
m_Projection(context, window->getWidth(), window->getHeight()){
    m_Window->createImGuiContext();

    m_GeometryProgram = m_TestShaderLibrary.loadShader("MVP", "ConstantBuffer");

    Mesh cube = createCube();
    m_CubeVertexArray = loadMesh(cube, m_GeometryProgram);

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

    auto lerp = glm::vec4(m_Lerp, m_TextureResample, 0.0f, 0.0f);
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
    Texture2DImageDesc texDesc;
    texDesc.image = createResourceView(img);
    m_AwesomeFaceTexture = m_Context->createTexture2D(texDesc);

    auto fbDesc = m_Context->createFrameBufferLayout();
    ViewportDesc vpDesc;
    vpDesc.width = window->getWidth();
    vpDesc.height = window->getHeight();
    vpDesc.minDepth = 0.0f;
    vpDesc.maxDepth = 1.0f;
    vpDesc.xPos = 0;
    vpDesc.yPos = 0;
    fbDesc->addViewportDesc(vpDesc);
    ColorAttachmentDesc colorDesc;
    colorDesc.format = SR_TEXTURE_RGBA_F32;
    colorDesc.width = window->getWidth();
    colorDesc.height = window->getHeight();
    fbDesc->addColorAttachmentDesc(colorDesc);
    DepthStencilAttachmentDesc depthDesc;
    depthDesc.format = SR_TEXTURE_DEPTH_24_STENCIL_8;
    depthDesc.width = window->getWidth();
    depthDesc.height = window->getHeight();
    depthDesc.enableDepthTest = m_DepthTest;
    depthDesc.enableShaderAccess = true;
    fbDesc->addDepthStencilAttachmentDesc(depthDesc);
    m_FrameBuffer = m_Context->createFrameBuffer(fbDesc);

    m_ScreenProgram = m_TestShaderLibrary.loadShader("Texture", "Screen");
    Mesh rectangle = createRectangle();
    m_ScreenVertexArray = loadMesh(rectangle, m_ScreenProgram);

}

TestFrameBuffer::~TestFrameBuffer() {
    m_Window->destroyImGuiContext();
}

void TestFrameBuffer::onUpdate() {
    auto currentTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
    m_DeltaTime = currentTime - m_LastFrameTime;
    m_LastFrameTime = currentTime;

    render();

}

void TestFrameBuffer::onEvent(const Event &event) {
    m_Camera.update(event, m_DeltaTime);
    if (event.type == SR_EVENT_WINDOW_RESIZED){
        auto width = event.windowWidth;
        auto height = event.windowHeight;
        m_Projection.onResize(width, height);
        m_Context->onResize(width, height);
        m_FrameBuffer->onResize(width, height);
    }
}

void TestFrameBuffer::render() {
    m_Sampler->bind(0);

    m_Context->beginRenderPass(m_FrameBuffer);
    m_AwesomeFaceTexture->bindShaderResource(0);

    m_ModelDataBuffer->bind(2);
    m_Camera.bind(1);
    m_Projection.bind(0);
    m_LerpBuffer->bind(4);
    m_GeometryProgram.shaderProgram->bind();
    m_Context->draw(m_CubeVertexArray);

    m_Context->endRenderPass(m_FrameBuffer);

    m_Context->beginRenderPass();
    if (m_SampleFromColorAttachment){
        m_FrameBuffer->getColorAttachment(0)->bindShaderResource(0);
    }
    else{
        m_FrameBuffer->getDepthStencilAttachment()->bindShaderResource(0);
    }
    m_ScreenProgram.shaderProgram->bind();
    m_Context->draw(m_ScreenVertexArray);

    renderImGui();

    m_Context->endRenderPass();
}

void TestFrameBuffer::renderImGui() {
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
        auto lerp = glm::vec4(m_Lerp, m_TextureResample, 0.0f, 0.0f);
        m_LerpBuffer->setData(&lerp);
    }
    if (ImGui::SliderFloat("Texture Resample", &m_TextureResample, 1.0f, 10.0f)){
        auto lerp = glm::vec4(m_Lerp, m_TextureResample, 0.0f, 0.0f);
        m_LerpBuffer->setData(&lerp);
    }
    if (ImGui::Checkbox("Depth test", &m_DepthTest)){
        m_FrameBuffer->enableDepthTest(m_DepthTest);
    }
    if (ImGui::Checkbox("Depth Mash", &m_DepthMask)){
        m_Context->getDefaultFrameBuffer()->setDepthMask(static_cast<SR_DEPTH_MASK>(m_DepthMask));
    }
    ImGui::Checkbox("Sample from color attachment", &m_SampleFromColorAttachment);

    if (ImGui::Button("Save")){
        auto img = m_FrameBuffer->getColorAttachment(0)->getData();
        ImageFileDesc desc;
        desc.fileName = "test.png";
        img->writeToFile(desc);
    }

    // memory allocation tracker
    ImGui::Columns(2, "Memory Allocation Tracker");
    ImGui::Separator();
    ImGui::Text("Total Allocated"); ImGui::NextColumn();
    ImGui::Text("%d bytes", getAllocatedMemory()); ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("Total Freed"); ImGui::NextColumn();
    ImGui::Text("%d bytes", getFreedMemory()); ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("Usage"); ImGui::NextColumn();
    ImGui::Text("%d bytes", getMemoryUsage()); ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Columns(1);
    ImGui::End();

    m_Window->onImGuiEnd();
}