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

    class SR_API MouseEvent: public Event{
    protected:
        explicit MouseEvent(SR_EVENT_TYPE type);

    public:
        MouseEvent() = delete;

    };

    class SR_API MouseMovedEvent: public MouseEvent{
    public:
        MouseMovedEvent(int32 posX, int32 posY);
    };

    class SR_API MouseScrolledEvent: public MouseEvent{
    public:
        MouseScrolledEvent(int32 scrollX, int32 scrollY);
    };

    class SR_API MouseButtonPressedEvent: public MouseEvent{
    public:
        explicit MouseButtonPressedEvent(SR_MOUSE_BUTTON button);
    };

    class SR_API MouseButtonReleasedEvent: public MouseEvent{
    public:
        explicit MouseButtonReleasedEvent(SR_MOUSE_BUTTON button);
    };

    class SR_API MouseEnteredEvent: public MouseEvent{
    public:
        MouseEnteredEvent();
    };

    class SR_API MouseLeftEvent: public MouseEvent{
    public:
        MouseLeftEvent();
    };

    class SR_API RawMouseMovedEvent: public MouseEvent{
    public:
        RawMouseMovedEvent(int32 posX, int32 posY);
    };

}

