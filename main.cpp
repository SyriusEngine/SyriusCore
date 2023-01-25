#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Driver.hpp"
#include "Tests/FramebufferTest.hpp"

using namespace Syrius;

void messageCallback(const Syrius::Message& msg){
    if (msg.message.find("Device Extension") == std::string::npos){
        std::string message = "[Syrius: " + msg.function + "]: severity = " + getMessageSeverityString(msg.severity) + ", type = " +
                              getMessageTypeString(msg.messageType) + "\n";
        message += "File = " + msg.file + ", line = " + std::to_string(msg.line) + "\n";
        message += "Message = " + msg.message + "\n";
        std::cerr << message;
        std::cerr << "\n\n--------------------------------------------------------------------------------\n";
    }
}

int test(){
    try{
        syriusCoreInit();
        setDebugMessageCallback(messageCallback);
        testFramebuffer(Syrius::SR_API_OPENGL);
        testFramebuffer(Syrius::SR_API_D3D11);
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
    try{

        syriusCoreInit();
        setDebugMessageCallback(messageCallback);

        WindowDesc wDesc;
        wDesc.xPos = 200;
        wDesc.yPos = 200;
        wDesc.width = 1280;
        wDesc.height = 720;
        wDesc.title = " The pubes, farts and other spices";

        auto window = createWindow(wDesc);
        ContextDesc cDesc;
        cDesc.api = Syrius::SR_API_D3D11;

        auto context = window->createContext(cDesc);
        context->setVerticalSynchronisation(true);
        window->createImGuiContext();

        auto rect = createRectangle();
        auto screen = createRectangle();
        auto shader = loadShader("./Resources/Shaders/HLSL/Basic-vs.hlsl", "./Resources/Shaders/HLSL/Basic-fs.hlsl", Syrius::SR_SHADER_CODE_HLSL, context);
        auto screenShader = loadShader("./Resources/Shaders/HLSL/Screen-vs.hlsl", "./Resources/Shaders/HLSL/Screen-fs.hlsl", Syrius::SR_SHADER_CODE_HLSL, context);
        auto vao1 = loadMesh(rect, shader, context);
//        vao1->setDrawMode(Syrius::SR_DRAW_LINES_STRIP);

        auto face = createImage("./Resources/Textures/awesomeface.png");
        auto logo = createImage("./Resources/Textures/insta.png");

        SamplerDesc splrDesc;
        splrDesc.wrapU = Syrius::SR_TEXTURE_WRAP_MIRROR_REPEAT;
        splrDesc.wrapV = Syrius::SR_TEXTURE_WRAP_MIRROR_REPEAT;
        auto sampler = context->createSampler(splrDesc);

        Texture2DImageDesc texDesc;
        texDesc.image = face.createView();
        texDesc.sampler = sampler;

        auto texture = context->createTexture2D(texDesc);

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

            context->beginRenderPass();

            shader.shaderProgram->bind();
            sampler->bind(0);
            texture->bind(0);
            vao1->drawBuffers();

            window->onImGuiBegin();




            ImGui::Begin("Hello, world!");
            static char str0[128] = "Hello, world!";
            ImGui::InputText("string", str0, IM_ARRAYSIZE(str0));

            static float move[3] = {0.0f, 0.0f, 0.0f};
            ImGui::DragFloat3("Move", move, 0.01f);

            ImGui::End();

            window->onImGuiEnd();

            context->endRenderPass();

            context->swapBuffers();
        }

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