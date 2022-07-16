#pragma once

#include "Event.hpp"

namespace Syrius{

    class SR_API WindowEvent: public Event{
    protected:

        explicit WindowEvent(SR_EVENT_TYPE type);

    public:
        WindowEvent() = delete;

    };

    class SR_API WindowMovedEvent: public WindowEvent{
    public:
        WindowMovedEvent(int32_t posX, int32_t posY);
    };

    class SR_API WindowResizedEvent: public WindowEvent{
    public:
        WindowResizedEvent(int32_t width, int32_t height);
    };

    class SR_API WindowOpenedEvent: public WindowEvent{
    public:
        WindowOpenedEvent();
    };

    class SR_API WindowClosedEvent: public WindowEvent{
    public:
        WindowClosedEvent();
    };

    class SR_API WindowLostFocusEvent: public WindowEvent{
    public:
        WindowLostFocusEvent();
    };

    class SR_API WindowGainedFocusEvent: public WindowEvent{
    public:
        WindowGainedFocusEvent();
    };

    class SR_API WindowFileDroppedEvent: public WindowEvent{
    public:
        explicit WindowFileDroppedEvent(const std::string& filePath);
    };

}

