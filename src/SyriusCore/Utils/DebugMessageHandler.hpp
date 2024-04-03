#pragma once

#include "PlatformInclude.hpp"
#include <cassert>
#include "../../../include/SyriusCore/Utils/Message.hpp"

namespace Syrius{

    class DebugMessageHandler{
    public:

        static void setDebugMessageHandler(handleDebugMessageFunc newHandler);

        static void pushOpenGLMessageCallback(GLenum source, uint32 type, uint32 id, uint32 severity, int32 length, const char* message, const void* userParam);

        static void pushOpenGlError(GLenum error, const std::string& function, const std::string& file, uint32 line);

        template<typename... Args>
        static void pushMessage(SR_CORE_MESSAGE_SEVERITY severity, SR_CORE_MESSAGE_SOURCE type, const std::string& function, const std::string& file, uint32 line, const std::string& message, Args... args){
            Size formatSize = std::snprintf(nullptr, 0, message.c_str(), args...);
            std::vector<byte> formatBuffer(formatSize + 1);
            std::snprintf(reinterpret_cast<char*>(formatBuffer.data()), formatSize + 1, message.c_str(), args...);
            Message msg;
            msg.severity = severity;
            msg.messageType = type;
            msg.function = function;
            msg.file = file;
            msg.line = line;
            msg.message = std::string(reinterpret_cast<char*>(formatBuffer.data()));
            if (m_MessageHandler){
                m_MessageHandler(msg);
            }
            else{
                throw std::runtime_error("No message handler set!");
            }
        }

        template<typename... Args>
        static void throwMessage(const std::string& function, const std::string& file, uint32 line, const std::string& message, Args... args){
            Size formatSize = std::snprintf(nullptr, 0, message.c_str(), args...);
            std::vector<byte> formatBuffer(formatSize + 1);
            std::snprintf(reinterpret_cast<char*>(formatBuffer.data()), formatSize + 1, message.c_str(), args...);
            std::string msg = std::string(reinterpret_cast<char*>(formatBuffer.data()));
            std::string exc = "EXCEPTION THROWN -> [" + file + ":" + function + ":" + std::to_string(line) + "] " + msg;
            throw std::runtime_error(exc);
        }

        template<typename... Args>
        static void syriusAssert(SR_CORE_MESSAGE_SOURCE source, const char* condition, const std::string& function, const std::string& file, uint32 line, const std::string& message, Args... args){
            Size formatSize = std::snprintf(nullptr, 0, message.c_str(), args...);
            std::vector<byte> formatBuffer(formatSize + 1);
            std::snprintf(reinterpret_cast<char*>(formatBuffer.data()), formatSize + 1, message.c_str(), args...);
            Message msg;
            msg.severity = SR_CORE_MESSAGE_SEVERITY_HIGH;
            msg.messageType = source;
            msg.function = function;
            msg.file = file;
            msg.line = line;
            msg.message = "Condition: " + std::string(condition) + ", Message = " + std::string(reinterpret_cast<char*>(formatBuffer.data()));
            if (m_MessageHandler){
                m_MessageHandler(msg);
            }
            _assert(msg.message.c_str(), file.c_str(), line);
        }


#if defined(SR_CORE_PLATFORM_WIN64)
        static void formatHresultMessage(HRESULT hr, const std::string& function, const std::string& file, uint32 line);

        static void d3d11DeviceRemovedHandler(HRESULT hr, const std::string& function, const std::string& file, uint32 line);

#if defined(SR_COMPILER_MSVC)
        static void dxgiGetMessages();
#endif
#elif defined(SR_CORE_PLATFORM_LINUX)
        static int32 x11ErrorHandler(Display* display, XErrorEvent* event);

#endif

    private:
        static handleDebugMessageFunc m_MessageHandler;
#if defined(SR_COMPILER_MSVC)
        static uint64 m_DxgiMessageIndex;
        static struct IDXGIInfoQueue* m_DxgiInfoQueue;
#endif

    };

}
