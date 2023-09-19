#include "Utils.hpp"

void printEventInfo(const Event& event){
    switch (event.type){
        case SR_EVENT_WINDOW_CLOSED:
            std::cout << "Window closed event" << std::endl;
            break;
        case SR_EVENT_WINDOW_RESIZED:
            std::cout << "Window resized event: " << event.windowWidth << "x" << event.windowHeight << std::endl;
            break;
        case SR_EVENT_WINDOW_MOVED:
            std::cout << "Window moved event: " << event.windowPosX << "x" << event.windowPosY << std::endl;
            break;
        case SR_EVENT_WINDOW_GAINED_FOCUS:
            std::cout << "Window focus gained event" << std::endl;
            break;
        case SR_EVENT_WINDOW_LOST_FOCUS:
            std::cout << "Window focus lost event" << std::endl;
            break;
        case SR_EVENT_WINDOW_FILE_DROPPED:
            std::cout << "Window file dropped event: " << event.droppedFilePath << std::endl;
            break;
        case SR_EVENT_MOUSE_ENTERED:
            std::cout << "Window mouse entered event" << std::endl;
            break;
        case SR_EVENT_MOUSE_LEFT:
            std::cout << "Window mouse left event" << std::endl;
            break;
        case SR_EVENT_MOUSE_MOVED:
            std::cout << "Window mouse moved event: " << event.mousePosX << "x" << event.mousePosY << std::endl;
            break;
        case SR_EVENT_MOUSE_SCROLLED:
            std::cout << "Window mouse scrolled event: " << event.mouseScrollX << "x" << event.mouseScrollY << std::endl;
            break;
        case SR_EVENT_MOUSE_BUTTON_PRESSED:
            std::cout << "Window mouse button pressed event: " << event.mouseButton << std::endl;
            break;
        case SR_EVENT_MOUSE_BUTTON_RELEASED:
            std::cout << "Window mouse button released event: " << event.mouseButton << std::endl;
            break;
        case SR_EVENT_KEYBOARD_KEY_PRESSED:
            std::cout << "Window keyboard key pressed event: " << event.keyCode << std::endl;
            break;
        case SR_EVENT_KEYBOARD_KEY_RELEASED:
            std::cout << "Window keyboard key released event: " << event.keyCode << std::endl;
            break;
        case SR_EVENT_KEYBOARD_KEY_TYPED:
            std::cout << "Window keyboard key typed event: " << event.keyCode << std::endl;
            break;
        default:
            std::cout << "Unknown event" << std::endl;
            break;
    }
}

void printContextInfo(const ResourceView<Context>& context){
    std::string deviceInfo = "\n";
    deviceInfo += "Device name                          : " + context->getDeviceName() + "\n";
    deviceInfo += "API Version                          : " + context->getAPIVersion() + "\n";
    deviceInfo += "Shading language version             : " + context->getShadingLanguageVersion() + "\n";
    deviceInfo += "Max Framebuffer width                : " + std::to_string(context->getMaxFramebufferWidth()) + "\n";
    deviceInfo += "Max Framebuffer width                : " + std::to_string(context->getMaxFramebufferHeight()) + "\n";
    deviceInfo += "Max framebuffer texture attachments  : " + std::to_string(context->getMaxFramebufferTextureAttachments()) + "\n";
    deviceInfo += "Max Texture Slots                    : " + std::to_string(context->getMaxTextureSlots()) + " slots\n";
    deviceInfo += "Max Texture2D Size                   : " + std::to_string(context->getMaxTexture2DSize()) + " bytes\n";
    deviceInfo += "Max constant buffer size             : " + std::to_string(context->getMaxConstantBufferSize()) + " bytes\n";
    deviceInfo += "Depth buffer depth                   : " + std::to_string(context->getMaxDepthBufferBits()) + " bits\n";

    std::cout << deviceInfo << std::endl;
}
