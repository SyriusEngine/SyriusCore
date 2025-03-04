#include "Layer.hpp"
#include <chrono>
#include <SyriusUtils/SyriusUtils.hpp>

Layer::Layer(ResourceView<Context>& context, const UP<SyriusWindow> &window, EasyIni::Configuration& config):
m_Context(context),
m_Window(window),
m_Config(config),
m_CurrentX(0.0f),
m_CurrentY(0.0f),
m_DeltaTime(0.0f),
m_LastFrameTime(0.0f),
m_ShaderLibrary(config["Context"]["ShaderLibraryPath"].getOrDefault("./Resources/Shaders"), context){
    m_Window->createImGuiContext();
    addImGuiDrawFunction([this]{
        imGuiDebugPanel(m_Context);
    });
}

Layer::~Layer() {
    m_Window->destroyImGuiContext();
}

void Layer::onUpdate() {
    auto currentTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
    m_DeltaTime = currentTime - m_LastFrameTime;
    m_LastFrameTime = currentTime;
}

void Layer::onEvent(const Event &event) {

}


ResourceView<VertexArray> Layer::loadMesh(Mesh &mesh, ShaderProgram &program) {
    auto layout = m_Context->createVertexLayout();
    layout->addAttribute("Position", SR_FLOAT32_3);
    layout->addAttribute("Color", SR_FLOAT32_3);
    layout->addAttribute("Normal", SR_FLOAT32_3);
    layout->addAttribute("Tangent", SR_FLOAT32_3);
    layout->addAttribute("TexCoord", SR_FLOAT32_2);

    VertexBufferDesc vboDesc;
    vboDesc.usage = SR_BUFFER_USAGE_DEFAULT;
    vboDesc.data = &mesh.vertices[0];
    vboDesc.layout = layout;
    vboDesc.count = mesh.vertices.size();
    auto vbo = m_Context->createVertexBuffer(vboDesc);

    IndexBufferDesc iboDesc;
    iboDesc.data = &mesh.indices[0];
    iboDesc.count = mesh.indices.size();
    iboDesc.usage = SR_BUFFER_USAGE_DEFAULT;
    iboDesc.dataType = SR_UINT32;
    auto ibo = m_Context->createIndexBuffer(iboDesc);

    VertexArrayDesc vaoDesc;
    vaoDesc.drawMode = SR_DRAW_TRIANGLES;
    vaoDesc.vertexShader = program.vertexShader;
    vaoDesc.vertexBuffer = vbo;
    vaoDesc.indexBuffer = ibo;
    auto vao = m_Context->createVertexArray(vaoDesc);

    return vao;
}

void Layer::renderImGui() {
    m_Window->onImGuiBegin();

    m_CurrentX = 0.0f;
    m_CurrentY = 0.0f;
    m_CurrentColumnWidth = &m_Column1Width;
    for(auto& draw : m_ImGuiDraw){
        draw();
    }

    m_Window->onImGuiEnd();
}

void Layer::addImGuiDrawFunction(DrawFunction drawFunction) {
    m_ImGuiDraw.push_back(drawFunction);
}

void Layer::imGuiDebugPanel(ResourceView<Context>& context) {
    imGuiBeginPanel("Debug Panel");

    ImGui::ColorPicker3("Background Color", context->getDefaultFrameBuffer()->getColorAttachment(0)->getClearColor());

    ImGui::Columns(2, "Memory Allocation Tracker");
    ImGui::Separator();
    ImGui::Text("Total Allocated"); ImGui::NextColumn();
    ImGui::Text("%d bytes", MemoryProfiler::getAllocatedMemory()); ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("Total Freed"); ImGui::NextColumn();
    ImGui::Text("%d bytes",  MemoryProfiler::getDeallocatedMemory()); ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Text("Usage"); ImGui::NextColumn();
    ImGui::Text("%d bytes",  MemoryProfiler::getCurrentUsage()); ImGui::NextColumn();
    ImGui::Separator();
    ImGui::Columns(1);
    imGuiEndPanel();
}

void Layer::imGuiIndexBufferPanel(ResourceView<IndexBuffer> & indexBuffer) {
    imGuiBeginPanel("Index Buffer Panel");

    static bool useRectangle = true;
    if (ImGui::Checkbox("Draw Rectangle", &useRectangle)){
        if (!useRectangle){
            Mesh triangle = createTriangle();
            indexBuffer->setData(triangle.indices.data(), triangle.indices.size());
        }
        else{
            Mesh rectangle = createRectangle();
            indexBuffer->setData(rectangle.indices.data(), rectangle.indices.size());
        }
    }
    if (ImGui::Button("Read Data")){
        auto pData = indexBuffer->getData();
        auto u32Data = reinterpret_cast<u32*>(pData.get());
        for(int i = 0; i < indexBuffer->getCount(); i++){
            std::cout << u32Data[i];
            if (i != indexBuffer->getCount() - 1){
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
    imGuiEndPanel();
}

void Layer::imGuiVertexArrayPanel(ResourceView<VertexArray> & vertexArray) {
    static std::vector<std::string> drawModes = {"SR_DRAW_POINTS", "SR_DRAW_LINES", "SR_DRAW_LINE_STRIP",
                                                 "SR_DRAW_TRIANGLES", "SR_DRAW_TRIANGLE_STRIP"};
    static int drawModeIndex = 3;
    imGuiBeginPanel("Vertex Array Panel");
    auto drawMode = vertexArray->getDrawMode();
    ImGui::Text("Draw Mode: %s", drawModeToString(drawMode).c_str());
    if (ImGui::BeginCombo("Draw Mode", drawModes[drawModeIndex].c_str())){
        for(int i = 0; i < drawModes.size(); i++){
            bool isSelected = (drawModeIndex == i);
            if (ImGui::Selectable(drawModes[i].c_str(), isSelected)){
                drawModeIndex = i;
                vertexArray->setDrawMode(static_cast<SR_DRAW_MODE>(i + 1));
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    imGuiEndPanel();
}

void Layer::imGuiRenderTransformConstantBuffer(ResourceView<ConstantBuffer>&  constantBuffer) {
    static auto translation = glm::vec3(0.0f);
    static auto rotation = glm::vec3(0.0f);
    static auto sca = glm::vec3(1.0f);
    auto update = [&]{
        glm::mat4 translate  = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 rotate = glm::toMat4(glm::quat(rotate));
        glm::mat4 scale = glm::scale(sca);

        glm::mat4 transformation = translate * scale; // TODO: Add rotation (somehow it is not working rn)
        constantBuffer->setData(&transformation, sizeof(glm::mat4));
    };

    imGuiBeginPanel("Transform Constant Buffer");
    if (ImGui::SliderFloat3("Translation", &translation.x, -1.0f, 1.0f)){
        update();
    }
    if (ImGui::SliderFloat3("Rotation", &rotation.x, -1.0f, 1.0f)){
        update();
    }
    if (ImGui::SliderFloat3("Scale", &sca.x, 0.0f, 5.0f)){
        update();
    }
    imGuiEndPanel();
}

void Layer::imGuiSamplerPanel(ResourceView<Sampler> & sampler) {
    static std::vector<std::string> filterModes = {"SR_TEXTURE_FILTER_LINEAR", "SR_TEXTURE_FILTER_POINT"};
    static int minFilterIndex = 0;
    static int magFilterIndex = 0;
    static std::vector<std::string> wrapModes = {"SR_TEXTURE_WRAP_REPEAT", "SR_TEXTURE_WRAP_MIRROR_REPEAT",
                                                 "SR_TEXTURE_WRAP_CLAMP_EDGE", "SR_TEXTURE_WRAP_CLAMP_BORDER"};
    static int wrapUModeIndex = 0;
    static int wrapVModeIndex = 0;
    static int wrapWModeIndex = 0;
    imGuiBeginPanel("Sampler Panel");
    ImGui::Text("Min Filter Mode: %s", filterToString(sampler->getMinFilter()).c_str());
    ImGui::Text("Mag Filter Mode: %s", filterToString(sampler->getMagFilter()).c_str());
    ImGui::Text("Wrap U Mode: %s", wrapModeToString(sampler->getWrapU()).c_str());
    ImGui::Text("Wrap V Mode: %s", wrapModeToString(sampler->getWrapV()).c_str());
    ImGui::Text("Wrap W Mode: %s", wrapModeToString(sampler->getWrapW()).c_str());
    if (ImGui::BeginCombo("Min Filter", filterModes[minFilterIndex].c_str())){
        for(int i = 0; i < filterModes.size(); i++){
            bool isSelected = (minFilterIndex == i);
            if (ImGui::Selectable(filterModes[i].c_str(), isSelected)){
                minFilterIndex = i;
                sampler->setFilter(static_cast<SR_TEXTURE_FILTER>(minFilterIndex + 1), sampler->getMagFilter());
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Mag Filter", filterModes[magFilterIndex].c_str())){
        for(int i = 0; i < filterModes.size(); i++){
            bool isSelected = (magFilterIndex == i);
            if (ImGui::Selectable(filterModes[i].c_str(), isSelected)){
                magFilterIndex = i;
                sampler->setFilter(sampler->getMinFilter(), static_cast<SR_TEXTURE_FILTER>(magFilterIndex + 1));
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Wrap U", wrapModes[wrapUModeIndex].c_str())){
        for(int i = 0; i < wrapModes.size(); i++){
            bool isSelected = (wrapUModeIndex == i);
            if (ImGui::Selectable(wrapModes[i].c_str(), isSelected)){
                wrapUModeIndex = i;
                sampler->setWrap(static_cast<SR_TEXTURE_WRAP>(wrapUModeIndex + 1), sampler->getWrapV(), sampler->getWrapW());
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Wrap V", wrapModes[wrapVModeIndex].c_str())){
        for(int i = 0; i < wrapModes.size(); i++){
            bool isSelected = (wrapVModeIndex == i);
            if (ImGui::Selectable(wrapModes[i].c_str(), isSelected)){
                wrapVModeIndex = i;
                sampler->setWrap(sampler->getWrapU(), static_cast<SR_TEXTURE_WRAP>(wrapVModeIndex + 1), sampler->getWrapW());
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Wrap W", wrapModes[wrapWModeIndex].c_str())){
        for(int i = 0; i < wrapModes.size(); i++){
            bool isSelected = (wrapWModeIndex == i);
            if (ImGui::Selectable(wrapModes[i].c_str(), isSelected)){
                wrapWModeIndex = i;
                sampler->setWrap(sampler->getWrapU(), sampler->getWrapV(), static_cast<SR_TEXTURE_WRAP>(wrapWModeIndex + 1));
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    auto borderColorPtr = sampler->getBorderColor();
    float borderColor[4] = {borderColorPtr[0], borderColorPtr[1], borderColorPtr[2], borderColorPtr[3]};
    if (ImGui::ColorPicker3("Border Color", borderColor)){
        sampler->setBorderColor(borderColor);
    }


    imGuiEndPanel();
}

void Layer::imGuiTextureParametersPanel(ResourceView<ConstantBuffer> &constantBuffer) {
    static TextureParameters texParams;
    imGuiBeginPanel("Texture Parameters");
    if (ImGui::SliderFloat("Lerp Factor", &texParams.lerpFactor, 0.0f, 1.0f)){
        constantBuffer->setData(&texParams, sizeof(TextureParameters));
    }
    if (ImGui::SliderFloat("Scale", &texParams.scale, 0.0f, 20.0f)){
        constantBuffer->setData(&texParams, sizeof(TextureParameters));
    }
    imGuiEndPanel();
}

void Layer::imGuiTexturePanel(ResourceView<Texture2D> &texture) {
    static float checkerBoardColor[4] = {1.0f, 0.0f, 1.0f, 1.0f};
    const u32 checkerBoardMaxWidth = 512;
    const u32 checkerBoardMaxHeight = 512;
    static u32 checkerBoardStartX = 0;
    static u32 checkerBoardStartY = 0;
    static u32 checkerBoardWidth = 256;
    static u32 checkerBoardHeight = 256;

    auto updateTexture = [&]{
        auto color = createCheckerBoard(checkerBoardColor, checkerBoardWidth, checkerBoardHeight);
        if (!color.empty()){
            texture->setData(color.data(), checkerBoardStartX, checkerBoardStartY, checkerBoardWidth, checkerBoardHeight);
        }
    };


    imGuiBeginPanel("Texture Panel");
    // TODO: This broke when updating ImGui
    // ImGui::Image((void*)(intptr_t)texture->getIdentifier(), ImVec2(64, 64));
    ImGui::ColorPicker4("Checkerboard Color", checkerBoardColor);
    if (ImGui::SliderInt("Start X", (int*)&checkerBoardStartX, 0, checkerBoardMaxWidth)){
        if (checkerBoardStartX + checkerBoardWidth <= checkerBoardMaxWidth){
            updateTexture();
        }
    }
    if (ImGui::SliderInt("Start Y", (int*)&checkerBoardStartY, 0, checkerBoardMaxHeight)){
        if (checkerBoardStartY + checkerBoardHeight <= checkerBoardMaxHeight){
            updateTexture();
        }
    }
    if (ImGui::SliderInt("Width", (int*)&checkerBoardWidth, 0, checkerBoardMaxWidth)){
        if (checkerBoardStartX + checkerBoardWidth <= checkerBoardMaxWidth){
            updateTexture();
        }
    }
    if (ImGui::SliderInt("Height", (int*)&checkerBoardHeight, 0, checkerBoardMaxHeight)){
        if (checkerBoardStartY + checkerBoardHeight <= checkerBoardMaxHeight){
            updateTexture();
        }
    }

    imGuiEndPanel();

}

void Layer::imGuiDepthTestPanel(ResourceView<FrameBuffer> &frameBuffer) {
    static bool depthTest = false;
    static bool depthMask = true;

    imGuiBeginPanel("Depth Test Panel");
    if (ImGui::Checkbox("Depth test", &depthTest)){
        frameBuffer->enableDepthTest(depthTest);
    }
    if (ImGui::Checkbox("Depth Mask", &depthMask)){
        frameBuffer->setDepthMask(static_cast<SR_DEPTH_MASK>(depthMask));
    }
    imGuiEndPanel();
}

void Layer::imGuiCameraPanel(Camera &camera) {
    imGuiBeginPanel("Camera");
    if (camera.isEnabled()){
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Camera enabled");
    }
    else{
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Camera disabled");
    }
    imGuiEndPanel();
}

void Layer::imGuiBeginPanel(const char *name) const {
    ImGui::SetNextWindowPos(ImVec2(m_CurrentX, m_CurrentY));
    ImGui::SetNextWindowSize(ImVec2(*m_CurrentColumnWidth, 0.0f));
    ImGui::Begin(name);
}

void Layer::imGuiEndPanel() {
    auto size = ImGui::GetWindowSize();
    *m_CurrentColumnWidth = size.x;
    ImGui::End();
    if (m_CurrentY + size.y > m_Window->getHeight()){
        m_CurrentY = 0.0f;
        m_CurrentX = m_Window->getWidth() - 200.0f;
    }
    else{
        m_CurrentY += size.y;
    }
}

void Layer::imGuiFrameBufferPanel(ResourceView<FrameBuffer> &frameBuffer, i32& selectedTexture) {
    static bool depthTest = false;
    static std::vector<std::string> sampleFrom = {"Normal Color", "Inverted Color", "Depth Stencil Attachment"};
    static int sampleFromIndex  = 0;

    imGuiBeginPanel("Frame Buffer Panel");
    if (ImGui::Checkbox("Depth Test", &depthTest)){
        frameBuffer->enableDepthTest(depthTest);
    }
    if (ImGui::BeginCombo("Attachment", sampleFrom[sampleFromIndex].c_str())){
        for(int i = 0; i < sampleFrom.size(); i++){
            bool isSelected = (sampleFromIndex == i);
            if (ImGui::Selectable(sampleFrom[i].c_str(), isSelected)){
                sampleFromIndex = i;
                selectedTexture = i;
            }
            if (isSelected){
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    imGuiEndPanel();
}