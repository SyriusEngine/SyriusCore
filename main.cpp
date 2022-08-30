#include <iostream>
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
//    float m_TexCoords[2];
};

const std::vector<Vertex> vertices = {
    {-0.5f, -0.5f, 0.0f}, //{0.0f, 0.0f}},
    {0.5f, -0.5f, 0.0f}, //{1.0f, 0.0f}},
    {0.5f, 0.5f, 0.0f}, //{1.0f, 1.0f}},
    {-0.5f, 0.5f, 0.0f} //{0.0f, 1.0f}}
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
        auto context = window->createContext(SR_API_D3D11);
        context->setVerticalSynchronisation(true);
        context->setClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        window->createImGuiContext();

        auto layout = context->createVertexDescription();
        layout->addAttribute("Position", SR_FLOAT32_3);

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
