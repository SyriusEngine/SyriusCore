#include "Layer.hpp"

Layer::Layer(ResourceView<Context>& context, const Resource<SyriusWindow> &window, EasyIni::Configuration& config):
m_Context(context),
m_Window(window),
m_Config(config),
m_ShaderLibrary(config["Context"]["ShaderLibraryPath"].getOrDefault("./Resources/Shaders"), context){
    printContextInfo(m_Context);

}

Layer::~Layer() {

}

ResourceView<VertexArray> Layer::loadMesh(Mesh &mesh, ShaderProgram &program) {
    auto layout = m_Context->createVertexLayout();
    layout->addAttribute("Position", SR_FLOAT32_3);
    layout->addAttribute("Color", SR_FLOAT32_3);
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

    for(auto& draw : m_ImGuiDraw){
        draw();
    }

    m_Window->onImGuiEnd();
}

void Layer::addImGuiDrawFunction(DrawFunction drawFunction) {
    m_ImGuiDraw.push_back(drawFunction);

}

void Layer::imGuiDebugPanel(ResourceView<Context>& context) {
    ImGui::Begin("Debug Panel");

    ImGui::ColorPicker3("Background Color", context->getDefaultFrameBuffer()->getColorAttachment(0)->getClearColor());

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
}

