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
        cDesc.m_API = SR_API_OPENGL;
        ColorAttachmentDesc caDesc;
        caDesc.m_Width = 1280;
        caDesc.m_Height = 720;
        cDesc.m_DefaultFrameBufferDesc.m_ColorAttachments.push_back(caDesc);
        cDesc.m_DefaultFrameBufferDesc.m_Width = 1280;
        cDesc.m_DefaultFrameBufferDesc.m_Height = 720;

        auto context = window->createContext(cDesc);
        context->setClearColor(0.1, 0.2, 0.3, 1.0);

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