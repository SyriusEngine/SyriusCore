#pragma once

#include "Event.hpp"

namespace Syrius{

    typedef enum SR_KEYBOARD_KEY {
        SR_KEY_SPACE              = 32,
        SR_KEY_APOSTROPHE         = 39,  /* ' */
        SR_KEY_COMMA              = 44,  /* , */
        SR_KEY_MINUS              = 45,  /* - */
        SR_KEY_PERIOD             = 46,  /* . */
        SR_KEY_SLASH              = 47,  /* / */
        SR_KEY_0                  = 48,
        SR_KEY_1                  = 49,
        SR_KEY_2                  = 50,
        SR_KEY_3                  = 51,
        SR_KEY_4                  = 52,
        SR_KEY_5                  = 53,
        SR_KEY_6                  = 54,
        SR_KEY_7                  = 55,
        SR_KEY_8                  = 56,
        SR_KEY_9                  = 57,
        SR_KEY_SEMICOLON          = 59,  /* ; */
        SR_KEY_EQUAL              = 61,  /* = */
        SR_KEY_A                  = 65,
        SR_KEY_B                  = 66,
        SR_KEY_C                  = 67,
        SR_KEY_D                  = 68,
        SR_KEY_E                  = 69,
        SR_KEY_F                  = 70,
        SR_KEY_G                  = 71,
        SR_KEY_H                  = 72,
        SR_KEY_I                  = 73,
        SR_KEY_J                  = 74,
        SR_KEY_K                  = 75,
        SR_KEY_L                  = 76,
        SR_KEY_M                  = 77,
        SR_KEY_N                  = 78,
        SR_KEY_O                  = 79,
        SR_KEY_P                  = 80,
        SR_KEY_Q                  = 81,
        SR_KEY_R                  = 82,
        SR_KEY_S                  = 83,
        SR_KEY_T                  = 84,
        SR_KEY_U                  = 85,
        SR_KEY_V                  = 86,
        SR_KEY_W                  = 87,
        SR_KEY_X                  = 88,
        SR_KEY_Y                  = 89,
        SR_KEY_Z                  = 90,
        SR_KEY_LEFT_BRACKET       = 91,  /* [ */
        SR_KEY_BACKSLASH          = 92,  /* \ */
        SR_KEY_RIGHT_BRACKET      = 93,  /* ] */
        SR_KEY_GRAVE_ACCENT       = 96,  /* ` */
        SR_KEY_WORLD_1            = 161, /* non-US #1 */
        SR_KEY_WORLD_2            = 162, /* non-US #2 */
        SR_KEY_ESCAPE             = 256,
        SR_KEY_ENTER              = 257,
        SR_KEY_TAB                = 258,
        SR_KEY_BACKSPACE          = 259,
        SR_KEY_INSERT             = 260,
        SR_KEY_DELETE             = 261,
        SR_KEY_RIGHT              = 262,
        SR_KEY_LEFT               = 263,
        SR_KEY_DOWN               = 264,
        SR_KEY_UP                 = 265,
        SR_KEY_PAGE_UP            = 266,
        SR_KEY_PAGE_DOWN          = 267,
        SR_KEY_HOME               = 268,
        SR_KEY_END                = 269,
        SR_KEY_CAPS_LOCK          = 280,
        SR_KEY_SCROLL_LOCK        = 281,
        SR_KEY_NUM_LOCK           = 282,
        SR_KEY_PRINT_SCREEN       = 283,
        SR_KEY_PAUSE              = 284,
        SR_KEY_F1                 = 290,
        SR_KEY_F2                 = 291,
        SR_KEY_F3                 = 292,
        SR_KEY_F4                 = 293,
        SR_KEY_F5                 = 294,
        SR_KEY_F6                 = 295,
        SR_KEY_F7                 = 296,
        SR_KEY_F8                 = 297,
        SR_KEY_F9                 = 298,
        SR_KEY_F10                = 299,
        SR_KEY_F11                = 300,
        SR_KEY_F12                = 301,
        SR_KEY_F13                = 302,
        SR_KEY_F14                = 303,
        SR_KEY_F15                = 304,
        SR_KEY_F16                = 305,
        SR_KEY_F17                = 306,
        SR_KEY_F18                = 307,
        SR_KEY_F19                = 308,
        SR_KEY_F20                = 309,
        SR_KEY_F21                = 310,
        SR_KEY_F22                = 311,
        SR_KEY_F23                = 312,
        SR_KEY_F24                = 313,
        SR_KEY_NUMPAD_0           = 320,
        SR_KEY_NUMPAD_1           = 321,
        SR_KEY_NUMPAD_2           = 322,
        SR_KEY_NUMPAD_3           = 323,
        SR_KEY_NUMPAD_4           = 324,
        SR_KEY_NUMPAD_5           = 325,
        SR_KEY_NUMPAD_6           = 326,
        SR_KEY_NUMPAD_7           = 327,
        SR_KEY_NUMPAD_8           = 328,
        SR_KEY_NUMPAD_9           = 329,
        SR_KEY_KP_DECIMAL         = 330,
        SR_KEY_KP_DIVIDE          = 331,
        SR_KEY_KP_MULTIPLY        = 332,
        SR_KEY_KP_SUBTRACT        = 333,
        SR_KEY_KP_ADD             = 334,
        SR_KEY_KP_ENTER           = 335,
        SR_KEY_KP_EQUAL           = 336,
        SR_KEY_LEFT_SHIFT         = 340,
        SR_KEY_LEFT_CONTROL       = 341,
        SR_KEY_LEFT_ALT           = 342,
        SR_KEY_LEFT_SUPER         = 343,
        SR_KEY_RIGHT_SHIFT        = 344,
        SR_KEY_RIGHT_CONTROL      = 345,
        SR_KEY_RIGHT_ALT          = 346,
        SR_KEY_RIGHT_SUPER        = 347,
        SR_KEY_MENU               = 348,
        SR_KEY_NONE               = 0

    } SR_KEYBOARD_KEY;

    class SR_API KeyboardEvent: public Event{
    protected:
        explicit KeyboardEvent(SR_EVENT_TYPE type);

    public:
        KeyboardEvent() = delete;
    };

    class SR_API KeyTypedEvent: public KeyboardEvent{
    public:
        explicit KeyTypedEvent(char c);
    };

    class SR_API KeyPressedEvent: public KeyboardEvent{
    public:
        explicit KeyPressedEvent(SR_KEYBOARD_KEY key);
    };

    class SR_API KeyReleasedEvent: public KeyboardEvent{
    public:
        explicit KeyReleasedEvent(SR_KEYBOARD_KEY key);
    };

    class SR_API RawKeyPressedEvent: public KeyboardEvent{
    public:
        explicit RawKeyPressedEvent(SR_KEYBOARD_KEY key);
    };

    class SR_API RawKeyReleasedEvent: public KeyboardEvent{
    public:
        explicit RawKeyReleasedEvent(SR_KEYBOARD_KEY key);
    };

}

