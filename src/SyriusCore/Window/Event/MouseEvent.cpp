#include "../../../../include/SyriusCore/Window/Event/MouseEvent.hpp"

namespace Syrius{

    MouseEvent::MouseEvent(SR_EVENT_TYPE type)
    : Event(SR_EVENT_MOUSE, type){

    }

    MouseMovedEvent::MouseMovedEvent(int32_t posX, int32_t posY)
    : MouseEvent(SR_EVENT_MOUSE_MOVED){
        mousePosX = posX;
        mousePosY = posY;
    }

    MouseScrolledEvent::MouseScrolledEvent(int32_t scrollX, int32_t scrollY)
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

}