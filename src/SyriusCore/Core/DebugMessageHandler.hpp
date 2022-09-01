#pragma once

#include "PlatformInclude.hpp"
#include "../../../include/SyriusCore/Core/Message.hpp"

namespace Syrius{

    class SR_API DebugMessageHandler{
    public:

        static void setDebugMessageHandler(handleDebugMessageFunc newHandler);

        static void pushOpenGLMessageCallback(GLenum source, uint32 type, uint32 id, uint32 severity, int32 length, const char* message, const void* userParam);

        static void pushOpenGlError(GLenum error, const std::string& function, const std::string& file, uint32 line);

        static void pushMessage(SR_MESSAGE_SEVERITY severity, const std::string& message, const std::string& function, const std::string& file, uint32 line, SR_MESSAGE_TYPE type = SR_MESSAGE);

#if defined(SR_PLATFORM_WIN64)
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
