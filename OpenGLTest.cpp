#include "OpenGLTest.hpp"

namespace Test{

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

    int runOpenGLTest(){
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

            ShaderDesc sDesc;
            sDesc.m_VertexShader = vertexShader;
            sDesc.m_FragmentShader = fragmentShader;
            auto shaderProgram = context->createShader(sDesc);

            delete fragmentShader;
            delete vertexShader;

            auto imgLogo = createImage("./Resources/Textures/Logo.jpg", true);
            Texture2DDesc t2ddesc;
            t2ddesc.m_Image = imgLogo;
            t2ddesc.m_WrapAddressU = Syrius::SR_TEXTURE_WRAP_REPEAT;
            t2ddesc.m_WrapAddressV = SR_TEXTURE_WRAP_REPEAT;
            t2ddesc.m_MinFilter = Syrius::SR_TEXTURE_FILTER_LINEAR;
            t2ddesc.m_MagFilter = SR_TEXTURE_FILTER_LINEAR;
            auto texture = context->createTexture2D(t2ddesc);
            delete imgLogo;

            FrameBufferDesc fbDesc;
            fbDesc.m_Width = 1280;
            fbDesc.m_Height = 720;
            fbDesc.m_ColorAttachments.emplace_back();
            fbDesc.m_NumColorAttachments = 1;
            auto fbo = context->createFrameBuffer(fbDesc);
            fbo->setClearColor(0.6f, 0.5f, 0.2f, 1.0f);

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

                fbo->bind();
                fbo->clear();

                shaderProgram->bind();
                texture->bind(0);
                context->draw(vao);

                fbo->unbind();

                context->clear();

                context->getDefaultFrameBuffer()->bind();
                fbo->getColorAttachment(0)->bind(0);
                context->draw(vao);


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
}