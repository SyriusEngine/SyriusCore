#pragma once

#include "../../Core/SyriusCoreInclude.hpp"

namespace Syrius {

    typedef enum SR_EVENT_CLASS_TYPE {
        SR_EVENT_WINDOW =                   0x10,  // 0001 0000
        SR_EVENT_MOUSE =                    0x20,  // 0010 0000
        SR_EVENT_KEYBOARD =                 0x40,  // 0100 0000

    } SR_EVENT_CLASS_TYPE;

    typedef enum SR_EVENT_TYPE {

        SR_EVENT_NONE =                     0x00,  // 0000 0000

        SR_EVENT_WINDOW_MOVED =             0x11,  // 0001 0001
        SR_EVENT_WINDOW_RESIZED =           0x12,  // 0001 0010
        SR_EVENT_WINDOW_OPENED =            0x13,  // 0001 0011
        SR_EVENT_WINDOW_CLOSED =            0x14,  // 0001 0100
        SR_EVENT_WINDOW_LOST_FOCUS =        0x15,  // 0001 0101
        SR_EVENT_WINDOW_GAINED_FOCUS =      0x16,  // 0001 0110
        SR_EVENT_WINDOW_FILE_DROPPED =      0x17,  // 0001 0111

        SR_EVENT_MOUSE_MOVED =              0x21,  // 0010 0001
        SR_EVENT_MOUSE_SCROLLED =           0x22,  // 0010 0010
        SR_EVENT_MOUSE_BUTTON_PRESSED =     0x23,  // 0010 0011
        SR_EVENT_MOUSE_BUTTON_RELEASED =    0x24,  // 0010 0100
        SR_EVENT_MOUSE_ENTERED =            0x25,  // 0010 0101
        SR_EVENT_MOUSE_LEFT =               0x26,  // 0010 0110
        SR_EVENT_RAW_MOUSE_MOVED =          0x27,  // 0010 0111

        SR_EVENT_KEYBOARD_KEY_TYPED =       0x41,  // 0100 0001
        SR_EVENT_KEYBOARD_KEY_PRESSED =     0x42,  // 0100 0010
        SR_EVENT_KEYBOARD_KEY_RELEASED =    0x43,  // 0100 0011

    } SR_EVENT_TYPE;

    class SR_API Event{
    private:
        SR_EVENT_CLASS_TYPE m_EventClass;

    protected:

        Event(SR_EVENT_CLASS_TYPE classType, SR_EVENT_TYPE type);

    public:
        SR_EVENT_TYPE type;

        union {
            int32 windowPosX, windowWidth, mousePosX, mouseScrollX, mouseButton, keyCode = 0;
        };

        union {
            int32 windowPosY, windowHeight, mousePosY, mouseScrollY = 0;
        };

        char keyTyped = 0;
        std::string droppedFilePath;

    public:

        Event() = delete;

        ~Event();
    };
}

