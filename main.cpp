#include <iostream>


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SyriusCoreDev/SyriusCoreDev.hpp"
#include "Tests/FramebufferTest.hpp"

using namespace Syrius;

int test(){
    try{
        testFramebuffer(SR_API_OPENGL);
        testFramebuffer(SR_API_D3D11);
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

void run(){
    WindowDesc wDesc;
    wDesc.xPos = 200;
    wDesc.yPos = 200;
    wDesc.width = 1280;
    wDesc.height = 720;
    wDesc.title = " The pubes, farts and other spices";

    auto window = createWindow(wDesc);
    ContextDesc cDesc;
    cDesc.api = SR_API_D3D11;

    auto context = window->createContext(cDesc);
    context->setVerticalSynchronisation(true);
    window->createImGuiContext();

    auto rect = createRectangle();
    auto screen = createRectangle();
    auto shader = loadShader("./Resources/Shaders/HLSL/Basic.vert", "./Resources/Shaders/HLSL/Basic.frag", SR_SHADER_CODE_HLSL, context);
    auto screenShader = loadShader("./Resources/Shaders/HLSL/Screen.vert", "./Resources/Shaders/HLSL/Screen.frag", SR_SHADER_CODE_HLSL, context);
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
    splrDesc.wrapU = SR_TEXTURE_WRAP_MIRROR_REPEAT;
    splrDesc.wrapV = SR_TEXTURE_WRAP_MIRROR_REPEAT;
    auto sampler = context->createSampler(splrDesc);

    std::vector<ubyte> temp(512 * 1024 * 4);
    Texture2DDesc texDesc;
    texDesc.width = 1024;
    texDesc.height = 512;
    texDesc.format = SR_TEXTURE_RGBA_UI8;
    texDesc.data = temp.data();

    auto texture = context->createTexture2D(texDesc);
    texture->setData(face, 0, 0, 512, 512);
    texture->setData(logo, 512, 0, 512, 512);

    glm::mat4 model = glm::mat4(1.0f);
    ConstantBufferDesc cbDesc;
    cbDesc.size = sizeof(glm::mat4);
    cbDesc.data = &model[0];
    cbDesc.type = SR_BUFFER_DYNAMIC;
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
            if (event.type == SR_EVENT_WINDOW_CLOSED){
                window->close();
            }
            else if (event.type == SR_EVENT_WINDOW_RESIZED){
                context->onResize(event.windowWidth, event.windowHeight);
                fb->onResize(event.windowWidth, event.windowHeight);

            }
            else if (event.type == SR_EVENT_KEYBOARD_KEY_PRESSED){
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
}


int main(int argc, char** argv) {
    try {
        for (int i = 1; i < argc; ++i) {
            SyriusCoreDev dev(argv[i]);
            dev.run();
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }

    printf("Allocated Memory: %zu\n", getAllocatedMemory());
    printf("Freed Memory: %zu\n", getFreedMemory());

    return 0;
}