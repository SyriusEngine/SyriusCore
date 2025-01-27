#include "AppComponent.hpp"

void AppComponent::onEvent(Event &event) {

}

void AppComponent::onRender() {

}

void AppComponent::onImGuiRender(ImGuiSizeData& sizeData) {

}

AppComponent::AppComponent(UP<SyriusWindow>& window, ResourceView<Context> &context) :
m_Context(context),
m_Window(window){

}

ResourceView<VertexArray> AppComponent::loadMesh(Mesh &mesh, ShaderProgram &program) {
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

ShaderProgram AppComponent::loadShader(const std::string& vertexShader, const std::string& fragmentShader){
    if (vertexShader.empty()){
        throw std::runtime_error("ShaderLibrary::loadShader: Vertex shader name is empty");
    }
    if (fragmentShader.empty()){
        return loadShader(vertexShader, vertexShader);
    }

    std::string apiPath = "./Resources/Shaders";
    std::string extension;
    SR_SHADER_LANGUAGE_TYPE codeType;
    switch (m_Context->getType()){
        case SR_API_OPENGL: {
            apiPath += "/GLSL/";
            extension = ".glsl";
            codeType = SR_SHADER_LANGUAGE_GLSL;
            break;
        }
        case SR_API_D3D11:{
            apiPath += "/HLSL/";
            extension = ".hlsl";
            codeType = SR_SHADER_LANGUAGE_HLSL;
            break;
        }
        default:
            throw std::runtime_error("ShaderLibrary::loadShader: Unsupported API");
    }

    ShaderProgram program;

    ShaderModuleFileDesc vsDesc;
    vsDesc.shaderType = SR_SHADER_VERTEX;
    vsDesc.language = codeType;
    vsDesc.filePath = apiPath + vertexShader + "_vs" + extension;
    vsDesc.entryPoint = "main";
    program.vertexShader = m_Context->createShaderModule(vsDesc);

    ShaderModuleFileDesc fsDesc;
    fsDesc.shaderType = SR_SHADER_FRAGMENT;
    fsDesc.language = codeType;
    fsDesc.filePath = apiPath + fragmentShader + "_fs" + extension;
    fsDesc.entryPoint = "main";
    program.fragmentShader = m_Context->createShaderModule(fsDesc);

    ShaderDesc sDesc;
    sDesc.vertexShader = program.vertexShader;
    sDesc.fragmentShader = program.fragmentShader;
    program.shaderProgram = m_Context->createShader(sDesc);

    return program;
}

void AppComponent::imGuiBeginPanel(ImGuiSizeData &sizeData, const char *name) const {
    ImGui::SetNextWindowPos(ImVec2(sizeData.nextX, sizeData.nextY));
    ImGui::SetNextWindowSize(ImVec2(sizeData.nextWidth, sizeData.nextHeight));
    ImGui::Begin(name);
}

void AppComponent::imGuiEndPanel(ImGuiSizeData &sizeData) {
    auto windowSize = ImGui::GetWindowSize();
    sizeData.nextWidth = windowSize.x;
    if (sizeData.nextY + windowSize.y > m_Window->getHeight()){
        sizeData.nextY = 0.0f;
        sizeData.nextX = m_Window->getWidth() - sizeData.nextWidth;
    }
    else{
        sizeData.nextY += windowSize.y;
    }
    ImGui::End();
}