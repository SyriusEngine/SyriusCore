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
        cDesc.api = Syrius::SR_API_OPENGL;

        auto context = window->createContext(cDesc);
        context->setVerticalSynchronisation(true);
        window->createImGuiContext();

        auto rect = createRectangle();
        auto screen = createRectangle();
        auto shader = loadShader("./Resources/Shaders/GLSL/Basic.vert", "./Resources/Shaders/GLSL/Basic.frag", Syrius::SR_SHADER_CODE_GLSL, context);
        auto screenShader = loadShader("./Resources/Shaders/GLSL/Screen.vert", "./Resources/Shaders/GLSL/Screen.frag", Syrius::SR_SHADER_CODE_GLSL, context);
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
        splrDesc.wrapU = Syrius::SR_TEXTURE_WRAP_MIRROR_REPEAT;
        splrDesc.wrapV = Syrius::SR_TEXTURE_WRAP_MIRROR_REPEAT;
        auto sampler = context->createSampler(splrDesc);

        std::vector<ubyte> temp(512 * 1024 * 4);
        Texture2DDesc texDesc;
        texDesc.width = 1024;
        texDesc.height = 512;
        texDesc.format = SR_TEXTURE_DATA_FORMAT_RGBA_UI8;
        texDesc.sampler = sampler;
        texDesc.data = temp.data();

        auto texture = context->createTexture2D(texDesc);
        texture->setData(face, 0, 0, 512, 512);
        texture->setData(logo, 512, 0, 512, 512);

        glm::mat4 model = glm::mat4(1.0f);
        ConstantBufferDesc cbDesc;
        cbDesc.size = sizeof(glm::mat4);
        cbDesc.data = &model[0];
        cbDesc.type = Syrius::SR_BUFFER_DYNAMIC;
        cbDesc.name = "ModelMatrix";
        cbDesc.slot = 0;
        auto cb = context->createConstantBuffer(cbDesc);


        auto fbDesc = context->createFrameBufferDescription();
        ViewportDesc vpDesc;
        vpDesc.width = 1280;
        vpDesc.height = 720;
        fbDesc->addViewportDesc(vpDesc);
        ColorAttachmentDesc caDesc;
        caDesc.width = 1280;
        caDesc.height = 720;
        fbDesc->addColorAttachmentDesc(caDesc);
        DepthStencilAttachmentDesc dsDesc;
        dsDesc.width = 1280;
        dsDesc.height = 720;
        dsDesc.enableDepthTest = true;
        fbDesc->addDepthStencilAttachmentDesc(dsDesc);
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

            context->beginRenderPass(fb);

            shader.shaderProgram->bind();
            shader.shaderProgram->bind();
            sampler->bind(0);
            texture->bind(0);
            cb->bind();
            vao1->drawBuffers();
            vao2->drawBuffers();

            context->endRenderPass(fb);

            context->beginRenderPass();
            screenShader.shaderProgram->bind();
            fb->getColorAttachment(0)->bindShaderResource(0);
            screenVAO->drawBuffers();

            window->onImGuiBegin();




            ImGui::Begin("Hello, world!");
            static char str0[128] = "Hello, world!";
            ImGui::InputText("string", str0, IM_ARRAYSIZE(str0));

            static float move[3] = {0.0f, 0.0f, 0.0f};
            if (ImGui::SliderFloat3("Move", move, -1.0f, 1.0f)){
                glm::vec3 moveVec(move[0], move[1], move[2]);
                glm::mat4 translation = glm::translate(glm::mat4(1.0f), moveVec);
                cb->setData(&translation[0]);
            }

            ImGui::End();

            window->onImGuiEnd();

            context->endRenderPass();

            context->swapBuffers();
        }



    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }

    syriusCoreTerminate();

    printf("Allocated Memory: %zu\n", getAllocatedMemory());
    printf("Freed Memory: %zu\n", getFreedMemory());
    printf("Memory Usage: %zu\n", getMemoryUsage());

    return 0;
}