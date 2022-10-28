#include <iostream>
#define SR_IMPORT_DLL
#include "include/SyriusCore/SyriusCore.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace Syrius;

void messageCallback(const Syrius::Message& msg){
    if (msg.m_Message.find("Device Extension") == std::string::npos){
        std::string message = "[Syrius: " + msg.m_Function + "]: severity = " + getMessageSeverityString(msg.m_Severity) + ", type = " +
                              getMessageTypeString(msg.m_Type) + "\n";
        message += "File = " + msg.m_File + ", line = " + std::to_string(msg.m_Line) + "\n";
        message += "Message = " + msg.m_Message + "\n";
        std::cerr << message;
        std::cerr << "\n\n--------------------------------------------------------------------------------\n";
    }
}

struct Vertex{
    float m_Position[3];
    float m_TexCoords[2];
};

struct Mesh{
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
};

Mesh createSphere(uint32 rings, uint32 sectors){
    Mesh mesh;
    float const R = 1.0f/(float)(rings-1);
    float const S = 1.0f/(float)(sectors-1);
    uint32 r, s;

    for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            mesh.vertices.push_back({{x, y, z},  {s*S, r*R}});
        }

    for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
            mesh.indices.push_back(r * sectors + s);
            mesh.indices.push_back(r * sectors + (s+1));
            mesh.indices.push_back((r+1) * sectors + (s+1));

            mesh.indices.push_back(r * sectors + s);
            mesh.indices.push_back((r+1) * sectors + (s+1));
            mesh.indices.push_back((r+1) * sectors + s);
        }
    return mesh;
}


int main() {
    try{

        const SR_SUPPORTED_API api = Syrius::SR_API_D3D11;
        SR_SHADER_CODE_TYPE shaderCodeType = SR_SHADER_CODE_GLSL;
        std::string vertexShaderPath = "./Resources/Shaders/GLSL/Basic.vert";
        std::string fragmentShaderPath = "./Resources/Shaders/GLSL/Basic.frag";
        if (api == SR_API_D3D11){
            shaderCodeType = SR_SHADER_CODE_HLSL;
            vertexShaderPath = "./Resources/Shaders/HLSL/Basic-vs.hlsl";
            fragmentShaderPath = "./Resources/Shaders/HLSL/Basic-fs.hlsl";
        }


        syriusCoreInit();
        setDebugMessageCallback(messageCallback);

        Mesh sphere = createSphere(32, 32);

        WindowDesc wDesc;
        wDesc.m_PosX = 200;
        wDesc.m_PosY = 200;
        wDesc.m_Width = 1280;
        wDesc.m_Height = 720;
        wDesc.m_Title = " The pubes, farts and other spices";

        auto window = createWindow(wDesc);
        ContextDesc cDesc;
        cDesc.m_DefaultFrameBufferDesc.m_Width = 1280;
        cDesc.m_DefaultFrameBufferDesc.m_Height = 720;
        cDesc.m_API = api;
        auto context = window->createContext(cDesc);
        context->setVerticalSynchronisation(true);
        context->setClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        window->createImGuiContext();

        auto layout = context->createVertexDescription();
        layout->addAttribute("Position", SR_FLOAT32_3);
        layout->addAttribute("TexCoord", SR_FLOAT32_2);

        VertexBufferDesc vboDesc;
        vboDesc.m_Type = SR_BUFFER_DEFAULT;
        vboDesc.m_Data = &sphere.vertices[0];
        vboDesc.m_Layout = layout;
        vboDesc.m_Count = sphere.vertices.size();
        auto vbo = context->createVertexBuffer(vboDesc);

        IndexBufferDesc iboDesc;
        iboDesc.m_Data = &sphere.indices[0];
        iboDesc.m_Count = sphere.indices.size();
        iboDesc.m_Type = SR_BUFFER_DEFAULT;
        iboDesc.m_DataType = SR_UINT32;
        auto ibo = context->createIndexBuffer(iboDesc);

        ShaderModuleDesc vsDesc;
        vsDesc.m_Type = SR_SHADER_VERTEX;
        vsDesc.m_CodeType = shaderCodeType;
        vsDesc.m_Code = vertexShaderPath;
        vsDesc.m_LoadType = SR_LOAD_FROM_FILE;
        vsDesc.m_EntryPoint = "main";
        vsDesc.m_CodeLength = 0;
        auto vsm = context->createShaderModule(vsDesc);

        ShaderModuleDesc fsDesc;
        fsDesc.m_Type = SR_SHADER_FRAGMENT;
        fsDesc.m_CodeType = shaderCodeType;
        fsDesc.m_Code = fragmentShaderPath;
        fsDesc.m_LoadType = SR_LOAD_FROM_FILE;
        fsDesc.m_EntryPoint = "main";
        fsDesc.m_CodeLength = 0;
        auto fsm = context->createShaderModule(fsDesc);

        ShaderDesc sDesc;
        sDesc.m_VertexShader = vsm;
        sDesc.m_FragmentShader = fsm;
        auto shader = context->createShader(sDesc);

        VertexArrayDesc vaoDesc;
        vaoDesc.m_DrawMode = SR_DRAW_TRIANGLES;
        vaoDesc.m_VertexShader = vsm;
        vaoDesc.m_VertexBuffer = vbo;
        vaoDesc.m_IndexBuffer = ibo;
        auto vao = context->createVertexArray(vaoDesc);

        glm::mat4 transform = glm::mat4(1.0f);

        ConstantBufferDesc cbDesc;
        cbDesc.m_Size = sizeof(transform);
        cbDesc.m_Type = SR_BUFFER_DYNAMIC;
        cbDesc.m_Data = &transform[0];
        cbDesc.m_BlockName = "Transform";
        cbDesc.m_BindingIndex = 0;
        cbDesc.m_ShaderStage = SR_SHADER_VERTEX;
        auto cb = context->createConstantBuffer(cbDesc);

        Sampler2DDesc samplerDesc;
        samplerDesc.m_MinFilter = SR_TEXTURE_FILTER_LINEAR;
        samplerDesc.m_MagFilter = SR_TEXTURE_FILTER_LINEAR;

        auto sampler = context->createSampler2D(samplerDesc);

        auto img = createImage("./Resources/Textures/awesomeface.png");
        Texture2DDesc texDesc;
        texDesc.m_Image = img;
        texDesc.m_Sampler2D = sampler;
        auto texture1 = context->createTexture2D(texDesc);
        delete img;
        img = createImage("./Resources/Textures/Logo.jpg");
        texDesc.m_Image = img;
        auto texture2 = context->createTexture2D(texDesc);
        delete img;

        while (window->isOpen()){

            window->pollEvents();
            while (window->hasEvent()){
                auto event = window->getEvent();
                if (event.type == Syrius::SR_EVENT_WINDOW_CLOSED){
                    window->close();
                }
                else if (event.type == Syrius::SR_EVENT_WINDOW_RESIZED){
                    context->onResize(event.windowWidth, event.windowHeight);
                }
            }

            context->clear();
            cb->bind();
            sampler->bind(0);
            texture1->bind(0);
            texture2->bind(1);
            shader->bind();
            context->drawInstanced(vao, 100);

            window->onImGuiBegin();
            ImGui::Begin("Info");
            static float bgc[3] = {0.2f, 0.3f, 0.5f};
            if (ImGui::ColorPicker3("BackGroundColor", bgc)){
                context->setClearColor(bgc[0], bgc[1], bgc[2], 1.0f);
            }
            static glm::vec3 translation(0.0f, 0.0f, 0.0f);
            if (ImGui::SliderFloat3("Translation", &translation.x, -1.0f, 1.0f)){
                transform = glm::translate(glm::mat4(1.0f), translation);
                cb->setData(&transform[0]);
            }
            ImGui::Text("Allocated Memory: %zu", getAllocatedMemory());
            ImGui::Text("Freed Memory: %zu", getFreedMemory());
            ImGui::Text("Memory Usage: %zu", getMemoryUsage());
            ImGui::End();

            window->onImGuiEnd();

            context->swapBuffers();
        }


        window->destroyImGuiContext();
        window->destroyContext();
        destroyWindow(window);

        printf("Allocated Memory: %zu\n", getAllocatedMemory());
        printf("Freed Memory: %zu\n", getFreedMemory());
        printf("Memory Usage: %zu\n", getMemoryUsage());


        syriusCoreTerminate();


    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }
    return 0;
}
