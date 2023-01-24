#include "FramebufferTest.hpp"

void testFramebuffer(SR_SUPPORTED_API api){
    std::string vShader;
    std::string fShader;
    std::string vScreenShader;
    std::string fScreenShader;
    SR_SHADER_CODE_TYPE shaderType = SR_SHADER_CODE_GLSL;
    if (api == Syrius::SR_API_D3D11){
        vShader = "./Resources/Shaders/HLSL/Basic-vs.hlsl";
        fShader = "./Resources/Shaders/HLSL/Basic-fs.hlsl";
        vScreenShader = "./Resources/Shaders/HLSL/Screen-vs.hlsl";
        fScreenShader = "./Resources/Shaders/HLSL/Screen-fs.hlsl";
        shaderType = Syrius::SR_SHADER_CODE_HLSL;
    }
    else if (api == Syrius::SR_API_OPENGL){
        vShader = "./Resources/Shaders/GLSL/Basic.vert";
        fShader = "./Resources/Shaders/GLSL/Basic.frag";
        vScreenShader = "./Resources/Shaders/GLSL/Screen.vert";
        fScreenShader = "./Resources/Shaders/GLSL/Screen.frag";
        shaderType = Syrius::SR_SHADER_CODE_GLSL;
    }
    WindowDesc wDesc;
    wDesc.xPos = 200;
    wDesc.yPos = 200;
    wDesc.width = 1280;
    wDesc.height = 720;
    wDesc.title = " Framebuffer test - " + getAPIName(api);

    auto window = createWindow(wDesc);
    ContextDesc cDesc;
    cDesc.api = api;

    auto context = window->createContext(cDesc);
    context->setVerticalSynchronisation(true);
    window->createImGuiContext();

    auto rect = createRectangle();
    auto screen = createRectangle();
    auto shader = loadShader(vShader, fShader, shaderType, context);
    auto screenShader = loadShader(vScreenShader, fScreenShader, shaderType, context);

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
}
