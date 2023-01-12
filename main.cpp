#include <iostream>
#include <thread>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Driver.hpp"

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


int main() {
    try{

        syriusCoreInit();
        setDebugMessageCallback(messageCallback);

        WindowDesc wDesc;
        wDesc.m_PosX = 200;
        wDesc.m_PosY = 200;
        wDesc.m_Width = 1280;
        wDesc.m_Height = 720;
        wDesc.m_Title = " The pubes, farts and other spices";

        auto window = createWindow(wDesc);
        ContextDesc cDesc;
        cDesc.m_API = Syrius::SR_API_D3D11;
        ColorAttachmentDesc caDesc;
        caDesc.m_Width = 1280;
        caDesc.m_Height = 720;
        cDesc.m_DefaultFrameBufferDesc.m_ColorAttachments.push_back(caDesc);
        cDesc.m_DefaultFrameBufferDesc.m_Width = 1280;
        cDesc.m_DefaultFrameBufferDesc.m_Height = 720;

        auto context = window->createContext(cDesc);
        window->createImGuiContext();
        context->setClearColor(0.1, 0.2, 0.3, 1.0);

        auto rect = createRectangle();
        auto shader = loadShader("./Resources/Shaders/HLSL/Basic-vs.hlsl", "./Resources/Shaders/HLSL/Basic-fs.hlsl", Syrius::SR_SHADER_CODE_HLSL, context);

        for (auto& v: rect.vertices){
            v.m_Position[0] += 0.5;
            if (v.m_TexCoords[0] == 1.0f){
                v.m_TexCoords[0] = 0.5f;
            }
        }
        auto vao1 = loadMesh(rect, shader, context);

        for (auto& v: rect.vertices){
            v.m_Position[0] -= 1.0;
            if (v.m_TexCoords[0] == 0.5f){
                v.m_TexCoords[0] = 1.0f;
            }
            if (v.m_TexCoords[0] == 0.0f){
                v.m_TexCoords[0] = 0.5f;
            }
        }
        auto vao2 = loadMesh(rect, shader, context);


        auto face = createImage("./Resources/Textures/awesomeface.png");
        auto logo = createImage("./Resources/Textures/insta.png");

        SamplerDesc splrDesc;
        auto sampler = context->createSampler(splrDesc);

        std::vector<ubyte> temp(512 * 1024 * 4);

        Texture2DDesc texDesc;
        texDesc.m_Width = 1024;
        texDesc.m_Height = 512;
        texDesc.m_Format = SR_TEXTURE_DATA_FORMAT_RGBA_UI8;
        texDesc.m_Sampler = sampler;
        texDesc.m_Data = temp.data();

        auto texture = context->createTexture2D(texDesc);
        texture->setData(face, 0, 0, 512, 512);
        texture->setData(logo, 512, 0, 512, 512);



        while (window->isOpen()){

            window->pollEvents();
            while (window->hasEvent()){
                auto event = window->getEvent();
                if (event.type == Syrius::SR_EVENT_WINDOW_CLOSED){
                    window->close();
                }
                else if (event.type == Syrius::SR_EVENT_WINDOW_RESIZED){

                }
                else if (event.type == Syrius::SR_EVENT_KEYBOARD_KEY_PRESSED){
                    if (event.keyCode == SR_KEY_F){
                        if (!window->isFullscreen()){
                            window->enableFullscreen();
                        }
                        else{
                            window->disableFullscreen();
                        }
                    }
                }
            }

            context->clear();

            context->beginRenderPass();
            shader.shaderProgram->bind();
            sampler->bind(0);
            texture->bind(0);
            context->draw(vao1);
            context->draw(vao2);

            context->endRenderPass();


            window->onImGuiBegin();

            ImGui::Begin("Hello, world!");
            static char str0[128] = "Hello, world!";
            ImGui::InputText("string", str0, IM_ARRAYSIZE(str0));

            ImGui::End();

            window->onImGuiEnd();

            context->swapBuffers();
        }

        destroyWindow(window);

        syriusCoreTerminate();

        printf("Allocated Memory: %zu\n", getAllocatedMemory());
        printf("Freed Memory: %zu\n", getFreedMemory());
        printf("Memory Usage: %zu\n", getMemoryUsage());



    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }
    return 0;
}