#pragma once

#include "PlatformInclude.hpp"
#include "../../../include/SyriusCore/Core/Message.hpp"

namespace Syrius{

    class SR_CORE_API DebugMessageHandler{
    public:

        static void setDebugMessageHandler(handleDebugMessageFunc newHandler);

        static void pushOpenGLMessageCallback(GLenum source, uint32 type, uint32 id, uint32 severity, int32 length, const char* message, const void* userParam);

        static void pushOpenGlError(GLenum error, const std::string& function, const std::string& file, uint32 line);

        template<typename... Args>
        static void pushMessage(SR_MESSAGE_SEVERITY severity, SR_MESSAGE_TYPE type, const std::string& function, const std::string& file, uint32 line, const std::string& message, Args... args){
            Size formatSize = std::snprintf(nullptr, 0, message.c_str(), args...);
            std::vector<byte> formatBuffer(formatSize + 1);
            std::snprintf(reinterpret_cast<char*>(formatBuffer.data()), formatSize + 1, message.c_str(), args...);
            Message msg;
            msg.m_Severity = severity;
            msg.m_Type = type;
            msg.m_Function = function;
            msg.m_File = file;
            msg.m_Line = line;
            msg.m_Message = std::string(reinterpret_cast<char*>(formatBuffer.data()));
            if (m_MessageHandler){
                m_MessageHandler(msg);
            }
            else{
                throw std::runtime_error("No message handler set!");
            }
        }

#if defined(SR_CORE_PLATFORM_WIN64)
        static void formatHresultMessage(HRESULT hr, const std::string& function, const std::string& file, uint32 line);

        static void d3d11DeviceRemovedHandler(HRESULT hr, const std::string& function, const std::string& file, uint32 line);

#if defined(SR_COMPILER_MSVC)
        static void dxgiGetMessages();
#endif

#endif
        static void vulkanFormatVkResultMessage(VkResult result, const std::string& message, const std::string& function, const std::string& file, uint32 line);

        static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);

    private:
        static handleDebugMessageFunc m_MessageHandler;
#if defined(SR_COMPILER_MSVC)
        static uint64 m_DxgiMessageIndex;
        static struct IDXGIInfoQueue* m_DxgiInfoQueue;
#endif

    };

}
