#pragma once

#include "../../Utils/SyriusCoreInclude.hpp"

typedef enum SR_EVENT_CLASS_TYPE {
    SR_EVENT_WINDOW =                   0x10,  // 0001 0000
    SR_EVENT_MOUSE =                    0x20,  // 0010 0000
    SR_EVENT_KEYBOARD =                 0x40,  // 0100 0000

} SR_EVENT_CLASS_TYPE;

typedef enum SR_EVENT_TYPE {

    SR_EVENT_NONE =                     0x00,

    SR_EVENT_WINDOW_MOVED =             0x11,
    SR_EVENT_WINDOW_RESIZED =           0x12,
    SR_EVENT_WINDOW_OPENED =            0x13,
    SR_EVENT_WINDOW_CLOSED =            0x14,
    SR_EVENT_WINDOW_LOST_FOCUS =        0x15,
    SR_EVENT_WINDOW_GAINED_FOCUS =      0x16,
    SR_EVENT_WINDOW_FILE_DROPPED =      0x17,
    SR_EVENT_WINDOW_REFRESHED =         0x18,

    SR_EVENT_MOUSE_MOVED =              0x21,
    SR_EVENT_MOUSE_SCROLLED =           0x22,
    SR_EVENT_MOUSE_BUTTON_PRESSED =     0x23,
    SR_EVENT_MOUSE_BUTTON_RELEASED =    0x24,
    SR_EVENT_MOUSE_ENTERED =            0x25,
    SR_EVENT_MOUSE_LEFT =               0x26,
    SR_EVENT_RAW_MOUSE_MOVED =          0x27,

    SR_EVENT_KEYBOARD_KEY_TYPED =       0x41,
    SR_EVENT_KEYBOARD_KEY_PRESSED =     0x42,
    SR_EVENT_KEYBOARD_KEY_RELEASED =    0x43,
    SR_EVENT_RAW_KEYBOARD_KEY_PRESSED = 0x45,
    SR_EVENT_RAW_KEYBOARD_KEY_RELEASED = 0x46,

} SR_EVENT_TYPE;

namespace Syrius {

    class SR_CORE_API Event{
    private:
        SR_EVENT_CLASS_TYPE m_EventClass;

    protected:

        Event(SR_EVENT_CLASS_TYPE classType, SR_EVENT_TYPE type);

    public:
        SR_EVENT_TYPE type;

        union {
            i32 windowPosX, windowWidth, mousePosX, mouseScrollX, mouseDeltaX, mouseButton, keyCode = 0;
        };

        union {
            i32 windowPosY, windowHeight, mousePosY, mouseScrollY, mouseDeltaY = 0;
        };

        char keyTyped = 0;
        std::string droppedFilePath;

    public:

        Event() = delete;

        ~Event();
    };
}

