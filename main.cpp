#include <iostream>
#include "include/SyriusCore/SyriusCore.hpp"

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

struct Vec4{
    float m_X;
    float m_Y;
    float m_Z;
    float m_W;
};

const std::vector<Vertex> vertices = {
    {{-0.05f, -0.05f, 0.0f}, {0.0f, 0.0f}},
    {{0.05f, -0.05f, 0.0f}, {1.0f, 0.0f}},
    {{0.05f, 0.05f, 0.0f}, {1.0f, 1.0f}},
    {{-0.05f, 0.05f, 0.0f}, {0.0f, 1.0f}}
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
        window->createImGuiContext();

        context->setClearColor(0.2f, 0.3f, 0.8f, 1.0f);

        auto vd = context->createVertexDescription();
        vd->addAttribute("Position", SR_FLOAT32_3);
        vd->addAttribute("TexCoords", SR_FLOAT32_2);

        VertexBufferDesc vboDesc;
        vboDesc.m_Type = Syrius::SR_BUFFER_DEFAULT;
        vboDesc.m_Layout = vd;
        vboDesc.m_Count = vertices.size();
        vboDesc.m_Data = &vertices[0];
        auto vbo = context->createVertexBuffer(vboDesc);

        IndexBufferDesc iboDesc;
        iboDesc.m_Type = SR_BUFFER_DEFAULT;
        iboDesc.m_DataType = SR_UINT32;
        iboDesc.m_Count = indices.size();
        iboDesc.m_Data = &indices[0];
        auto ibo = context->createIndexBuffer(iboDesc);

        VertexArrayDesc vaoDesc;
        vaoDesc.m_DrawMode = SR_DRAW_TRIANGLES;
        vaoDesc.m_IndexBuffer = ibo;
        vaoDesc.m_VertexBuffer = vbo;
        auto vao = context->createVertexArray(vaoDesc);

        ShaderModuleDesc vsmDesc;
        vsmDesc.m_Type = SR_SHADER_VERTEX;
        vsmDesc.m_EntryPoint = "main";
        vsmDesc.m_LoadType = SR_LOAD_FROM_FILE;
        vsmDesc.m_Code = "./Resources/Shaders/GLSL/Basic.vert";
        vsmDesc.m_CodeType = SR_SHADER_CODE_GLSL;
        auto vertexShader = context->createShaderModule(vsmDesc);

        ShaderModuleDesc fsmDesc;
        fsmDesc.m_Type = SR_SHADER_FRAGMENT;
        fsmDesc.m_EntryPoint = "main";
        fsmDesc.m_LoadType = SR_LOAD_FROM_FILE;
        fsmDesc.m_Code = "./Resources/Shaders/GLSL/Basic.frag";
        fsmDesc.m_CodeType = SR_SHADER_CODE_GLSL;
        auto fragmentShader = context->createShaderModule(fsmDesc);

        auto shaderProgram = context->createShader();
        shaderProgram->addShaderModule(vertexShader);
        shaderProgram->addShaderModule(fragmentShader);
        shaderProgram->link();

        delete fragmentShader;
        delete vertexShader;

        auto imgLogo = createImage("./Resources/Textures/Logo.jpg", true);
        auto texture = context->createTexture2D();
        texture->upload(imgLogo);
        delete imgLogo;

        Vec4 translations[100];
        int32 index = 0;
        for (int32 x =-10; x < 10; x += 2){
            for (int32 y = -10; y < 10; y += 2){
                translations[index].m_X = (float)x / 10.0f;
                translations[index].m_Y = (float)y / 10.0f;
                translations[index].m_Z = 0.0f;
                translations[index].m_W = 1.0f;
                index++;
            }
        }

        ConstantBufferDesc cbDesc;
        cbDesc.m_Type = SR_BUFFER_DYNAMIC;
        cbDesc.m_BindingIndex = 0;
        cbDesc.m_Size = sizeof(translations);
        cbDesc.m_BlockName = "Transformation";
        cbDesc.m_Data = translations;
        auto cbo = context->createConstantBuffer(cbDesc);
        cbo->addShader(shaderProgram);

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

            shaderProgram->bind();
            cbo->bind();
            texture->bind(0);
            context->drawInstanced(vao, 100);

            window->onImGuiBegin();
            ImGui::Begin("Info");
            static float background[3] = {0.2f, 0.3f, 0.8f};
            if (ImGui::ColorPicker3("Background", background)){
                context->setClearColor(background[0], background[1], background[2], 1.0f);
            }
            ImGui::End();
            context->onImGuiEnd();

            context->swapBuffers();
        }


        window->releaseImGuiContext();
        delete cbo;
        delete texture;
        delete shaderProgram;
        delete vao;
        delete ibo;
        delete vbo;
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
