#include "../../../../include/SyriusCore/Window/Event/WindowEvent.hpp"

namespace Syrius{

    WindowEvent::WindowEvent(SR_EVENT_TYPE type)
    : Event(SR_EVENT_WINDOW, type){

    }

    WindowMovedEvent::WindowMovedEvent(i32 posX, i32 posY)
    : WindowEvent(SR_EVENT_WINDOW_MOVED){
        windowPosX = posX;
        windowPosY = posY;
    }

    WindowResizedEvent::WindowResizedEvent(i32 width, i32 height)
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

    WindowRefreshedEvent::WindowRefreshedEvent():
    WindowEvent(SR_EVENT_WINDOW_REFRESHED){

    }


}