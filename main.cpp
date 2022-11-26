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

        Camera camera(0.3f, 0.1f);

        const SR_SUPPORTED_API api = Syrius::SR_API_OPENGL;
        SR_SHADER_CODE_TYPE shaderCodeType = SR_SHADER_CODE_GLSL;
        std::string vertexShaderPath = "./Resources/Shaders/GLSL/Basic.vert";
        std::string fragmentShaderPath = "./Resources/Shaders/GLSL/Basic.frag";
        std::string screenVertexShaderPath = "./Resources/Shaders/GLSL/Screen.vert";
        std::string screenFragmentShaderPath = "./Resources/Shaders/GLSL/Screen.frag";
        if (api == SR_API_D3D11){
            shaderCodeType = SR_SHADER_CODE_HLSL;
            vertexShaderPath = "./Resources/Shaders/HLSL/Basic-vs.hlsl";
            fragmentShaderPath = "./Resources/Shaders/HLSL/Basic-fs.hlsl";
            screenVertexShaderPath = "./Resources/Shaders/HLSL/Screen-vs.hlsl";
            screenFragmentShaderPath = "./Resources/Shaders/HLSL/Screen-fs.hlsl";
        }


        syriusCoreInit();
        setDebugMessageCallback(messageCallback);

        Mesh sphere = createRectangle(); //createSphere(32, 32);
        Mesh rectangle = createRectangle();

        WindowDesc wDesc;
        wDesc.m_PosX = 200;
        wDesc.m_PosY = 200;
        wDesc.m_Width = 1280;
        wDesc.m_Height = 720;
        wDesc.m_Title = " The pubes, farts and other spices";

        auto window = createWindow(wDesc);
        ColorAttachmentDesc caDesc;
        caDesc.m_Width = 1280;
        caDesc.m_Height = 720;
        ContextDesc cDesc;
        cDesc.m_DefaultFrameBufferDesc.m_Width = 1280;
        cDesc.m_DefaultFrameBufferDesc.m_Height = 720;
        cDesc.m_DefaultFrameBufferDesc.m_ColorAttachments = {caDesc};
        cDesc.m_API = api;
        auto context = window->createContext(cDesc);
        context->setVerticalSynchronisation(true);
        context->setClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        window->createImGuiContext();

        auto shader = loadShader(vertexShaderPath, fragmentShaderPath, shaderCodeType, context);
        auto screenShader = loadShader(screenVertexShaderPath, screenFragmentShaderPath, shaderCodeType, context);

        auto sphereVAO = loadMesh(sphere, shader, context);
        auto screenVAO = loadMesh(rectangle, screenShader, context);

        glm::mat4 transform = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);

        ConstantBufferDesc cbDesc;
        cbDesc.m_Size = sizeof(transform);
        cbDesc.m_Type = SR_BUFFER_DYNAMIC;
        cbDesc.m_Data = &transform[0];
        cbDesc.m_BlockName = "Transform";
        cbDesc.m_BindingIndex = 0;
        cbDesc.m_ShaderStage = SR_SHADER_VERTEX;
        auto cb = context->createConstantBuffer(cbDesc);

        SamplerDesc samplerDesc;
        samplerDesc.m_MinFilter = SR_TEXTURE_FILTER_LINEAR;
        samplerDesc.m_MagFilter = SR_TEXTURE_FILTER_LINEAR;

        auto sampler = context->createSampler(samplerDesc);

        auto img = createImage("./Resources/Textures/awesomeface.png");
        Texture2DDesc texDesc;
        texDesc.m_Image = img;
        auto texture1 = context->createTexture2D(texDesc);
        img = createImage("./Resources/Textures/Logo.jpg");
        texDesc.m_Image = img;
        auto texture2 = context->createTexture2D(texDesc);

        FrameBufferDesc fbDesc;
        fbDesc.m_ColorAttachments = {caDesc};
        fbDesc.m_Width = 1280;
        fbDesc.m_Height = 720;
        auto frameBuffer = context->createFrameBuffer(fbDesc);

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
                else if (event.type == Syrius::SR_EVENT_KEYBOARD_KEY_PRESSED){
                    if (event.keyCode == Syrius::SR_KEY_W){
                        camera.moveForward();
                    }
                    else if (event.keyCode == Syrius::SR_KEY_S){
                        camera.moveBackwards();
                    }
                    else if (event.keyCode == Syrius::SR_KEY_A){
                        camera.moveLeft();
                    }
                    else if (event.keyCode == Syrius::SR_KEY_D){
                        camera.moveRight();
                    }
                    else if (event.keyCode == Syrius::SR_KEY_ESCAPE){
                        window->close();
                    }
                    else if (event.keyCode == Syrius::SR_KEY_SPACE){
                        camera.moveUp();
                    }
                    else if (event.keyCode == Syrius::SR_KEY_LEFT_SHIFT){
                        camera.moveDown();
                    }
                    else if (event.keyCode == SR_KEY_F){
                        if (!window->isFullscreen()){
                            window->enableFullscreen();
                        }
                        else{
                            window->disableFullscreen();
                        }
                    }
                }
                else if (event.type == Syrius::SR_EVENT_RAW_MOUSE_MOVED){
                    camera.update(event.mousePosX, event.mousePosY);
                }
            }

            // first pass

            // for some reason D3D11 only shows the first frame, all subsequent frames are the clear color (=red)
            context->beginRenderPass(frameBuffer);
            sampler->bind(0);
            cb->bind();
            texture1->bind(0);
            texture2->bind(1);
            shader.shaderProgram->bind();
            context->draw(sphereVAO);
            context->endRenderPass(frameBuffer);

            context->beginRenderPass();
            screenShader.shaderProgram->bind();
            frameBuffer->getColorAttachment(0)->bind(0);
            context->draw(screenVAO);
            context->endRenderPass();




            window->onImGuiBegin();
            ImGui::Begin("Info");
            static float bgc[3] = {0.2f, 0.3f, 0.5f};
            if (ImGui::ColorPicker3("BackGroundColor", bgc)){
                context->setClearColor(bgc[0], bgc[1], bgc[2], 1.0f);
            }
            static glm::vec3 translation(0.0f, 0.0f, 0.0f);
            static glm::mat4 model = glm::mat4(1.0f);
            if (ImGui::SliderFloat3("Translation", &translation.x, -1.0f, 1.0f)){
                model = glm::translate(glm::mat4(1.0f), translation);
            }

            transform = model;
            cb->setData(&transform[0]);

            ImGui::Text("Allocated Memory: %zu", getAllocatedMemory());
            ImGui::Text("Freed Memory: %zu", getFreedMemory());
            ImGui::Text("Memory Usage: %zu", getMemoryUsage());
            ImGui::End();

            window->onImGuiEnd();

            context->endRenderPass();

            context->swapBuffers();

        }


        window->destroyImGuiContext();
        window->destroyContext();
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