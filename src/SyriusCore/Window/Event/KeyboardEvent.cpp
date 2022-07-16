#include "../../../../include/SyriusCore/Window/Event/KeyboardEvent.hpp"

namespace Syrius{

    KeyboardEvent::KeyboardEvent(SR_EVENT_TYPE type)
    : Event(SR_EVENT_KEYBOARD, type){

    }

    KeyTypedEvent::KeyTypedEvent(char c)
    : KeyboardEvent(SR_EVENT_KEYBOARD_KEY_TYPED){
        keyTyped = c;
    }

    KeyPressedEvent::KeyPressedEvent(SR_KEYBOARD_KEY key)
    : KeyboardEvent(SR_EVENT_KEYBOARD_KEY_PRESSED){
        keyCode = key;
    }

    KeyReleasedEvent::KeyReleasedEvent(SR_KEYBOARD_KEY key)
    : KeyboardEvent(SR_EVENT_KEYBOARD_KEY_RELEASED){
        keyCode = key;
    }
}