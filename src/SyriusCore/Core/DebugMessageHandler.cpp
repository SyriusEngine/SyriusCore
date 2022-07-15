#include "../../../include/SyirusCore/Core/DebugMessageHandler.hpp"
#include "PlatformInclude.hpp"

namespace Syrius{
    std::string getMessageTypeString(SR_MESSAGE_TYPE type){
        switch (type) {
            case SR_MESSAGE:
                return "MESSAGE";
            case SR_MESSAGE_HRESULT:
                return "HRESULT";
            case SR_MESSAGE_OPENGL:
                return "OPENGL";
            case SR_MESSAGE_VULKAN:
                return "VULKAN";
            case SR_MESSAGE_PRECONDITION:
                return "PRECONDITION";
            case SR_MESSAGE_POSTCONDITION:
                return "POSTCONDITION";
            case SR_MESSAGE_ASSERTION:
                return "ASSERTION";
            default:
                return "UNKNOWN";
        }
    }

    std::string getMessageSeverityString(SR_MESSAGE_SEVERITY severity){
        switch (severity) {
            case SR_MESSAGE_SEVERITY_HIGH:
                return "HIGH";
            case SR_MESSAGE_SEVERITY_MEDIUM:
                return "MEDIUM";
            case SR_MESSAGE_SEVERITY_LOW:
                return "LOW";
            case SR_MESSAGE_SEVERITY_INFO:
                return "INFO";
            default:
                return "UNKNOWN";
        }
    }

    void defaultMessageHandler(const Message& msg){
        std::string message = "[Syrius: " + msg.m_Function + "]: severity = " + getMessageSeverityString(msg.m_Severity) + ", type = " +
                              getMessageTypeString(msg.m_Type) + "\n";
        message += "File = " + msg.m_File + ", line = " + std::to_string(msg.m_Line) + "\n";
        message += "Message = " + msg.m_Message + "\n";
        std::cerr << message;
        std::cerr << "\n\n--------------------------------------------------------------------------------\n";
    }

    handleDebugMessageFunc DebugMessageHandler::m_MessageHandler = defaultMessageHandler;

    void DebugMessageHandler::setDebugMessageHandler(handleDebugMessageFunc newHandler) {
        m_MessageHandler = newHandler;
    }

    void DebugMessageHandler::pushOpenGLMessageCallback(uint32 source, uint32 type, uint32 id, uint32 severity, int32 length, const char* message, const void* userParam) {
        std::string msg = "type = ";
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:               msg += "Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: msg += "Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  msg += "Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         msg += "Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         msg += "Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              msg += "Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          msg += "Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           msg += "Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               msg += "Other"; break;
            default:                                msg += "UNKNOWN TYPE"; break;
        }
        msg += " (" + std::to_string(type) + ")\nsource = ";
        switch (source) {
            case GL_DEBUG_SOURCE_API:               msg += "API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     msg += "Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   msg += "Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:       msg += "Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:       msg += "Application"; break;
            case GL_DEBUG_SOURCE_OTHER:             msg += "Other"; break;
            default:                                msg += "UNKNOWN SOURCE"; break;
        }
        msg += " (" + std::to_string(source) + ")\n";
        msg += "Message length = " + std::to_string(length) + "\n";
        msg += "ID = " + std::to_string(id) + "\n";
        std::string sep = message;
        msg += "Message = " + sep + "\n";
        SR_MESSAGE_SEVERITY sev;
        switch (severity) {
            case GL_DEBUG_SEVERITY_LOW:             sev = SR_MESSAGE_SEVERITY_LOW; break;
            case GL_DEBUG_SEVERITY_MEDIUM:          sev = SR_MESSAGE_SEVERITY_MEDIUM; break;
            case GL_DEBUG_SEVERITY_HIGH:            sev = SR_MESSAGE_SEVERITY_HIGH; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:    sev = SR_MESSAGE_SEVERITY_INFO; break;
            default:                                sev = SR_MESSAGE_SEVERITY_INFO; break;
        }
        Message msgStruct;
        msgStruct.m_Function = "OpenGLCallback";
        msgStruct.m_File = "";
        msgStruct.m_Line = 0;
        msgStruct.m_Message = msg;
        msgStruct.m_Severity = sev;
        msgStruct.m_Type = SR_MESSAGE_OPENGL;
        m_MessageHandler(msgStruct);
    }

    void DebugMessageHandler::pushOpenGlError(uint32 error, const std::string &function, const std::string &file,
                                              uint32 line) {
        std::string msg = "OpenGL encountered a state error = ";
        switch (error) {
            case GL_INVALID_ENUM:                   msg += "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                  msg += "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:              msg += "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                 msg += "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:                msg += "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                  msg += "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  msg += "INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_CONTEXT_LOST:                   msg += "CONTEXT_LOST"; break;
            case GL_TABLE_TOO_LARGE:                msg += "TABLE_TOO_LARGE"; break;
            default:                                msg += "UNKNOWN ERROR"; break;
        }
        msg += " (" + std::to_string(error) + ")";
        Message msgStruct;
        msgStruct.m_Type = SR_MESSAGE_OPENGL;
        msgStruct.m_Severity = SR_MESSAGE_SEVERITY_HIGH;
        msgStruct.m_Message = msg;
        msgStruct.m_Function = function;
        msgStruct.m_File = file;
        msgStruct.m_Line = line;
        m_MessageHandler(msgStruct);
    }

    void DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY severity, const std::string& message, const std::string& function, const std::string& file, uint32 line, SR_MESSAGE_TYPE type) {
        Message msgStruct;
        msgStruct.m_Type = type;
        msgStruct.m_Severity = severity;
        msgStruct.m_Message = message;
        msgStruct.m_Function = function;
        msgStruct.m_File = file;
        msgStruct.m_Line = line;
        m_MessageHandler(msgStruct);

    }

#if defined(SR_PLATFORM_WIN64)
    void DebugMessageHandler::formatHresultMessage(int64 hr, const std::string& function, const std::string& file, uint32 line){
        LPTSTR errorText = nullptr;
        FormatMessage(
                FORMAT_MESSAGE_FROM_SYSTEM
                |FORMAT_MESSAGE_ALLOCATE_BUFFER
                |FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                hr,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&errorText,
                0,
                NULL);

        if (nullptr != errorText) {
            std::string msg = "Code = " + std::to_string(hr) + ", message = " + std::string(errorText);

            Message msgStruct;
            msgStruct.m_Type = SR_MESSAGE_HRESULT;
            msgStruct.m_Severity = SR_MESSAGE_SEVERITY_HIGH;
            msgStruct.m_Message = msg;
            msgStruct.m_Function = function;
            msgStruct.m_File = file;
            msgStruct.m_Line = line;
            m_MessageHandler(msgStruct);

            LocalFree(errorText);
            errorText = nullptr;

        }
    }

    void
    DebugMessageHandler::d3d11DeviceRemovedHandler(int64 hr, const std::string &function, const std::string &file,
                                                   uint32 line) {
        LPTSTR errorText = nullptr;
        FormatMessage(
                FORMAT_MESSAGE_FROM_SYSTEM
                |FORMAT_MESSAGE_ALLOCATE_BUFFER
                |FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                hr,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&errorText,
                0,
                NULL);

        if (nullptr != errorText) {
            std::string msg = "The application has thrown a DXGI_DEVICE_REMOVED exception. This indicates a driver crash"
                              "(or the graphics device is pulled away from the system). Detailed info: "
                              "Code = " + std::to_string(hr) + " msg = " + std::string(errorText);
            Message msgStruct;
            msgStruct.m_Type = SR_MESSAGE_HRESULT;
            msgStruct.m_Severity = SR_MESSAGE_SEVERITY_HIGH;
            msgStruct.m_Message = msg;
            msgStruct.m_Function = function;
            msgStruct.m_File = file;
            msgStruct.m_Line = line;
            m_MessageHandler(msgStruct);

            LocalFree(errorText);
            errorText = nullptr;

        }
    }

#endif
}