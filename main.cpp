#include <iostream>
#include "include/SyriusCore/SyriusCore.hpp"

int main() {
    try{
        Syrius::syriusInit();

        Syrius::WindowDesc wDesc;
        wDesc.m_PosX = 200;
        wDesc.m_PosY = 200;
        wDesc.m_Width = 800;
        wDesc.m_Height = 600;
        wDesc.m_Title = " The pubes, farts and other spices";

        auto window = Syrius::createWindow(wDesc);
        auto context = window->requestContext(Syrius::SR_API_VULKAN);

        window->requestImGuiContext();

        while (window->isOpen()){

            window->pollEvents();
            while (window->hasEvent()){
                auto event = window->getEvent();
                if (event.type == Syrius::SR_EVENT_WINDOW_CLOSED){
                    window->close();
                }
            }

//            window->onImGuiBegin();
//
//            ImGui::Begin("Test");
//            ImGui::End();
//
//            window->onImGuiEnd();

            window->update();
        }

        window->releaseImGuiContext();
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
