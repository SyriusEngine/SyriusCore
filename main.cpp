#include <iostream>
#include "include/SyriusCore/SyriusCore.hpp"

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
        Syrius::syriusInit();
        Syrius::setDebugMessageCallback(messageCallback);

        Syrius::WindowDesc wDesc;
        wDesc.m_PosX = 200;
        wDesc.m_PosY = 200;
        wDesc.m_Width = 800;
        wDesc.m_Height = 600;
        wDesc.m_Title = " The pubes, farts and other spices";

        auto window = Syrius::createWindow(wDesc);
        auto context = window->requestContext(Syrius::SR_API_VULKAN);

        while (window->isOpen()){

            window->pollEvents();
            while (window->hasEvent()){
                auto event = window->getEvent();
                if (event.type == Syrius::SR_EVENT_WINDOW_CLOSED){
                    window->close();
                }
            }


            window->update();
        }

        window->releaseContext();

        delete window;

        Syrius::syriusTerminate();

    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }
    return 0;
}
