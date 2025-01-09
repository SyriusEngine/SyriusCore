#pragma once

#include "Event.hpp"

namespace Syrius{

    class SR_CORE_API WindowEvent: public Event{
    protected:

        explicit WindowEvent(SR_EVENT_TYPE type);

    public:
        WindowEvent() = delete;

    };

    class SR_CORE_API WindowMovedEvent: public WindowEvent{
    public:
        WindowMovedEvent(i32 posX, i32 posY);
    };

    class SR_CORE_API WindowResizedEvent: public WindowEvent{
    public:
        WindowResizedEvent(i32 width, i32 height);
    };

    class SR_CORE_API WindowOpenedEvent: public WindowEvent{
    public:
        WindowOpenedEvent();
    };

    class SR_CORE_API WindowClosedEvent: public WindowEvent{
    public:
        WindowClosedEvent();
    };

    class SR_CORE_API WindowLostFocusEvent: public WindowEvent{
    public:
        WindowLostFocusEvent();
    };

    class SR_CORE_API WindowGainedFocusEvent: public WindowEvent{
    public:
        WindowGainedFocusEvent();
    };

    class SR_CORE_API WindowFileDroppedEvent: public WindowEvent{
    public:
        explicit WindowFileDroppedEvent(const std::string& filePath);
    };

}

