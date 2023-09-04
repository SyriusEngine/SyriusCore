#include "../../../include/SyriusCore/Utils/Message.hpp"

namespace Syrius{

    std::string getMessageTypeString(SR_CORE_MESSAGE_SOURCE type){
        switch (type) {
            case SR_CORE_MESSAGE:
                return "MESSAGE";
            case SR_CORE_MESSAGE_HRESULT:
                return "HRESULT";
            case SR_CORE_MESSAGE_OPENGL:
                return "OPENGL";
            case SR_CORE_MESSAGE_VULKAN:
                return "VULKAN";
            case SR_CORE_MESSAGE_D3D11:
                return "D3D11";
            case SR_CORE_MESSAGE_DXGI:
                return "DXGI";
            case SR_CORE_MESSAGE_WIN32:
                return "WIN32";
            case SR_CORE_MESSAGE_X11:
                return "X11";
            case SR_CORE_MESSAGE_PRECONDITION:
                return "PRECONDITION";
            case SR_CORE_MESSAGE_POSTCONDITION:
                return "POSTCONDITION";
            case SR_CORE_MESSAGE_ASSERTION:
                return "ASSERTION";
            default:
                return "UNKNOWN";
        }
    }

    std::string getMessageSeverityString(SR_CORE_MESSAGE_SEVERITY severity){
        switch (severity) {
            case SR_CORE_MESSAGE_SEVERITY_HIGH:
                return "HIGH";
            case SR_CORE_MESSAGE_SEVERITY_MEDIUM:
                return "MEDIUM";
            case SR_CORE_MESSAGE_SEVERITY_LOW:
                return "LOW";
            case SR_CORE_MESSAGE_SEVERITY_INFO:
                return "INFO";
            default:
                return "UNKNOWN";
        }
    }
}