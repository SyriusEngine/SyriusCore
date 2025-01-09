#pragma once

#include "Event.hpp"

namespace Syrius{

    typedef enum SR_MOUSE_BUTTON {

        SR_MOUSE_BUTTON_NONE =      0x00,
        SR_MOUSE_BUTTON_LEFT =      0x01,
        SR_MOUSE_BUTTON_MIDDLE =    0x02,
        SR_MOUSE_BUTTON_RIGHT =     0x03,
        SR_MOUSE_BUTTON_X1 =        0x04,
        SR_MOUSE_BUTTON_X2 =        0x05,

    } SR_MOUSE_BUTTON;

    class SR_CORE_API MouseEvent: public Event{
    protected:
        explicit MouseEvent(SR_EVENT_TYPE type);

    public:
        MouseEvent() = delete;

    };

    class SR_CORE_API MouseMovedEvent: public MouseEvent{
    public:
        MouseMovedEvent(i32 posX, i32 posY);
    };

    class SR_CORE_API MouseScrolledEvent: public MouseEvent{
    public:
        MouseScrolledEvent(i32 scrollX, i32 scrollY);
    };

    class SR_CORE_API MouseButtonPressedEvent: public MouseEvent{
    public:
        explicit MouseButtonPressedEvent(SR_MOUSE_BUTTON button);
    };

    class SR_CORE_API MouseButtonReleasedEvent: public MouseEvent{
    public:
        explicit MouseButtonReleasedEvent(SR_MOUSE_BUTTON button);
    };

    class SR_CORE_API MouseEnteredEvent: public MouseEvent{
    public:
        MouseEnteredEvent();
    };

    class SR_CORE_API MouseLeftEvent: public MouseEvent{
    public:
        MouseLeftEvent();
    };

    class SR_CORE_API RawMouseMovedEvent: public MouseEvent{
    public:
        RawMouseMovedEvent(i32 deltaX, i32 deltaY);
    };

}

