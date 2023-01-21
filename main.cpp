#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Driver.hpp"
#include "Tests/FramebufferTest.hpp"

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

int test(){
    try{
        syriusCoreInit();
        setDebugMessageCallback(messageCallback);
        testFramebufferOpenGL();
        testFramebufferD3D11();
        syriusCoreTerminate();
    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }
    printf("Allocated Memory: %zu\n", getAllocatedMemory());
    printf("Freed Memory: %zu\n", getFreedMemory());
    printf("Memory Usage: %zu\n", getMemoryUsage());
    return 0;

}


int main() {
    return test();
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

        auto context = window->createContext(cDesc);
        context->setVerticalSynchronisation(true);
        window->createImGuiContext();

        auto rect = createRectangle();
        auto screen = createRectangle();
        auto shader = loadShader("./Resources/Shaders/HLSL/Basic-vs.hlsl", "./Resources/Shaders/HLSL/Basic-fs.hlsl", Syrius::SR_SHADER_CODE_HLSL, context);
        auto screenShader = loadShader("./Resources/Shaders/HLSL/Screen-vs.hlsl", "./Resources/Shaders/HLSL/Screen-fs.hlsl", Syrius::SR_SHADER_CODE_HLSL, context);

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

        auto screenVAO = loadMesh(screen, screenShader, context);

        auto face = createImage("./Resources/Textures/awesomeface.png");
        auto logo = createImage("./Resources/Textures/insta.png");

        SamplerDesc splrDesc;
        splrDesc.m_WrapU = Syrius::SR_TEXTURE_WRAP_MIRROR_REPEAT;
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

        auto fbDesc = context->createFrameBufferDescription();
        ViewportDesc vpDesc;
        fbDesc->addViewportDesc(vpDesc);
        ColorAttachmentDesc caDesc;
        fbDesc->addColorAttachmentDesc(caDesc);
        auto fb = context->createFrameBuffer(fbDesc);



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

            fb->bind();
            fb->clear();
            shader.shaderProgram->bind();
            sampler->bind(0);
            texture->bind(0);
            vao1->drawBuffers();
            vao2->drawBuffers();
            fb->unbind();

            context->getDefaultFrameBuffer()->bind();
            context->getDefaultFrameBuffer()->clear();
            screenShader.shaderProgram->bind();
            fb->getColorAttachment(0)->bindShaderResource(0);
            screenVAO->drawBuffers();

            window->onImGuiBegin();



            ImGui::Begin("Hello, world!");
            static char str0[128] = "Hello, world!";
            ImGui::InputText("string", str0, IM_ARRAYSIZE(str0));

            static float move[3] = {0.0f, 0.0f, 0.0f};
            ImGui::DragFloat3("Move", move, 0.01f);

            ImGui::End();

            window->onImGuiEnd();

            context->swapBuffers();
        }

        destroyWindow(window);

        syriusCoreTerminate();



    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }

    printf("Allocated Memory: %zu\n", getAllocatedMemory());
    printf("Freed Memory: %zu\n", getFreedMemory());
    printf("Memory Usage: %zu\n", getMemoryUsage());

    return 0;
}