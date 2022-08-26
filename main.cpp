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
//    float m_Color[3];
};

const std::vector<float> vertices = {
    -0.5f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f
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
        wDesc.m_Width = 800;
        wDesc.m_Height = 600;
        wDesc.m_Title = " The pubes, farts and other spices";

        auto window = createWindow(wDesc);
        auto context = window->requestContext(SR_API_OPENGL);

        auto vd = context->createVertexDescription();
        vd->addAttribute("Position", SR_FLOAT32_3);

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


        while (window->isOpen()){

            window->pollEvents();
            while (window->hasEvent()){
                auto event = window->getEvent();
                if (event.type == Syrius::SR_EVENT_WINDOW_CLOSED){
                    window->close();
                }
            }

            test();

            shaderProgram->bind();
            vao->drawBuffers();


            window->update();
        }

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
