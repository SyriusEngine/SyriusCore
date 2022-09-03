#include <iostream>
#define SR_IMPORT_DLL
#include "include/SyriusCore/SyriusCore.hpp"
#include "OpenGLTest.hpp"

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

const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}}
};

const std::vector<uint32> indices = {
    0, 1, 2,
    2, 3, 0
};

int main() {
    try{
        syriusInit();
        setDebugMessageCallback(messageCallback);

        WindowDesc wDesc;
        wDesc.m_PosX = 200;
        wDesc.m_PosY = 200;
        wDesc.m_Width = 1280;
        wDesc.m_Height = 720;
        wDesc.m_Title = " The pubes, farts and other spices";

        auto window = createWindow(wDesc);
        auto context = window->createContext(SR_API_OPENGL);
        context->setVerticalSynchronisation(true);
        context->setClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        window->createImGuiContext();

        auto layout = context->createVertexDescription();
        layout->addAttribute("Position", SR_FLOAT32_3);
        layout->addAttribute("TexCoords", SR_FLOAT32_2);

        VertexBufferDesc vboDesc;
        vboDesc.m_Type = SR_BUFFER_DEFAULT;
        vboDesc.m_Data = &vertices[0];
        vboDesc.m_Layout = layout;
        vboDesc.m_Count = vertices.size();
        auto vbo = context->createVertexBuffer(vboDesc);

        IndexBufferDesc iboDesc;
        iboDesc.m_Data = &indices[0];
        iboDesc.m_Count = indices.size();
        iboDesc.m_Type = SR_BUFFER_DEFAULT;
        iboDesc.m_DataType = SR_UINT32;
        auto ibo = context->createIndexBuffer(iboDesc);

        ShaderModuleDesc vsDesc;
        vsDesc.m_Type = SR_SHADER_VERTEX;
        vsDesc.m_CodeType = SR_SHADER_CODE_GLSL;
        vsDesc.m_Code = "./Resources/Shaders/GLSL/Basic.vert";
        vsDesc.m_LoadType = SR_LOAD_FROM_FILE;
        vsDesc.m_EntryPoint = "main";
        vsDesc.m_CodeLength = 0;
        auto vsm = context->createShaderModule(vsDesc);

        ShaderModuleDesc fsDesc;
        fsDesc.m_Type = SR_SHADER_FRAGMENT;
        fsDesc.m_CodeType = SR_SHADER_CODE_GLSL;
        fsDesc.m_Code = "./Resources/Shaders/GLSL/Basic.frag";
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

        auto texImg = createImage("./Resources/Textures/Logo.jpg");
        Texture2DDesc tDesc;
        tDesc.m_Image = texImg;
        auto texture = context->createTexture2D(tDesc);

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

            shader->bind();
            texture->bind(0);
            context->draw(vao);

            window->onImGuiBegin();
            ImGui::Begin("Info");
            static float bgc[3] = {0.2f, 0.3f, 0.5f};
            if (ImGui::ColorPicker3("BackGroundColor", bgc)){
                context->setClearColor(bgc[0], bgc[1], bgc[2], 1.0f);
            }
            ImGui::End();

            window->onImGuiEnd();

            context->swapBuffers();
        }


        window->releaseImGuiContext();
        window->releaseContext();
        delete window;

        syriusTerminate();

    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }
    return 0;
}
