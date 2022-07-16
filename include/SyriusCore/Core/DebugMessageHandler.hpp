#pragma once

#include "SyriusCoreInclude.hpp"

namespace Syrius{

    typedef enum SR_MESSAGE_TYPE{
        SR_MESSAGE                  = 0x00,
        SR_MESSAGE_OPENGL           = 0x01,
        SR_MESSAGE_VULKAN           = 0x02,
        SR_MESSAGE_HRESULT          = 0x03,
        SR_MESSAGE_PRECONDITION     = 0x04,
        SR_MESSAGE_POSTCONDITION    = 0x05,
        SR_MESSAGE_ASSERTION        = 0x06,
    } SR_MESSAGE_TYPE;

    typedef enum SR_MESSAGE_SEVERITY{
        SR_MESSAGE_SEVERITY_INFO    = 0x00,
        SR_MESSAGE_SEVERITY_LOW    = 0x01,      // mostly used for messages that are not important
        SR_MESSAGE_SEVERITY_MEDIUM = 0x02,      // used for messages that are important, but not critical
        SR_MESSAGE_SEVERITY_HIGH   = 0x03,      // used for messages that are critical and (usually) results in program termination
    } SR_MESSAGE_SEVERITY;

    struct Message{
        SR_MESSAGE_TYPE m_Type;
        SR_MESSAGE_SEVERITY m_Severity;
        std::string m_Message;
        std::string m_Function;
        std::string m_File;
        uint32 m_Line;
    };

    typedef void(*handleDebugMessageFunc)(const Message&);

    std::string SR_API getMessageTypeString(SR_MESSAGE_TYPE type);

    std::string SR_API getMessageSeverityString(SR_MESSAGE_SEVERITY severity);

    class SR_API DebugMessageHandler{
    public:

        static void setDebugMessageHandler(handleDebugMessageFunc newHandler);

        static void pushOpenGLMessageCallback(uint32 source, uint32 type, uint32 id, uint32 severity, int32 length, const char* message, const void* userParam);

        static void pushOpenGlError(uint32 error, const std::string& function, const std::string& file, uint32 line);

        static void pushMessage(SR_MESSAGE_SEVERITY severity, const std::string& message, const std::string& function, const std::string& file, uint32 line, SR_MESSAGE_TYPE type = SR_MESSAGE);

#if defined(SR_PLATFORM_WIN64)
        static void formatHresultMessage(int64 hr, const std::string& function, const std::string& file, uint32 line);

        static void d3d11DeviceRemovedHandler(int64 hr, const std::string& function, const std::string& file, uint32 line);

#endif
        static void vulkanFormatVkResultMessage(int32 vkResult, const std::string& message, const std::string& function, const std::string& file, uint32 line);

    private:
        static handleDebugMessageFunc m_MessageHandler;

    };

}
