#include "../../../../include/SyriusCore/Window/Event/WindowEvent.hpp"

namespace Syrius{

    WindowEvent::WindowEvent(SR_EVENT_TYPE type)
    : Event(SR_EVENT_WINDOW, type){

    }

    WindowMovedEvent::WindowMovedEvent(int32_t posX, int32_t posY)
    : WindowEvent(SR_EVENT_WINDOW_MOVED){
        windowPosX = posX;
        windowPosY = posY;
    }

    WindowResizedEvent::WindowResizedEvent(int32_t width, int32_t height)
    : WindowEvent(SR_EVENT_WINDOW_RESIZED){
        windowWidth = width;
        windowHeight = height;
    }

    WindowOpenedEvent::WindowOpenedEvent()
    : WindowEvent(SR_EVENT_WINDOW_OPENED){

    }

    WindowClosedEvent::WindowClosedEvent()
    : WindowEvent(SR_EVENT_WINDOW_CLOSED){

    }

    WindowLostFocusEvent::WindowLostFocusEvent()
    : WindowEvent(SR_EVENT_WINDOW_LOST_FOCUS){

    }

    WindowGainedFocusEvent::WindowGainedFocusEvent()
    : WindowEvent(SR_EVENT_WINDOW_GAINED_FOCUS){

    }

    WindowFileDroppedEvent::WindowFileDroppedEvent(const std::string &filePath)
    : WindowEvent(SR_EVENT_WINDOW_FILE_DROPPED){
        droppedFilePath = filePath;
    }

}