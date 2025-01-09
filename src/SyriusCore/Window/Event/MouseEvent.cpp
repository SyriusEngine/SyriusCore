#include "../../../../include/SyriusCore/Window/Event/MouseEvent.hpp"

namespace Syrius{

    MouseEvent::MouseEvent(SR_EVENT_TYPE type)
    : Event(SR_EVENT_MOUSE, type){

    }

    MouseMovedEvent::MouseMovedEvent(i32 posX, i32 posY)
    : MouseEvent(SR_EVENT_MOUSE_MOVED){
        mousePosX = posX;
        mousePosY = posY;
    }

    MouseScrolledEvent::MouseScrolledEvent(i32 scrollX, i32 scrollY)
    : MouseEvent(SR_EVENT_MOUSE_SCROLLED){
        mouseScrollX = scrollX;
        mouseScrollY = scrollY;
    }

    MouseButtonPressedEvent::MouseButtonPressedEvent(SR_MOUSE_BUTTON button)
    : MouseEvent(SR_EVENT_MOUSE_BUTTON_PRESSED){
        mouseButton = button;
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(SR_MOUSE_BUTTON button)
    : MouseEvent(SR_EVENT_MOUSE_BUTTON_RELEASED){
        mouseButton = button;
    }

    MouseEnteredEvent::MouseEnteredEvent()
    : MouseEvent(SR_EVENT_MOUSE_ENTERED){

    }

    MouseLeftEvent::MouseLeftEvent()
    : MouseEvent(SR_EVENT_MOUSE_LEFT){

    }

    RawMouseMovedEvent::RawMouseMovedEvent(i32 deltaX, i32 deltaY)
    : MouseEvent(SR_EVENT_RAW_MOUSE_MOVED){
        mouseDeltaX = deltaX;
        mouseDeltaY = deltaY;

    }
}