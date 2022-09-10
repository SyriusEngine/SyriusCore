#pragma once

#include "DebugMessageHandler.hpp"

#define SR_CORE_MESSAGE(message) \
    DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_INFO, message, SR_FUNC, SR_FILE, SR_LINE);

#define SR_CORE_WARNING(message) \
    DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_MEDIUM, message, SR_FUNC, SR_FILE, SR_LINE);

#define SR_CORE_EXCEPTION(message) \
    DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_HIGH, message, SR_FUNC, SR_FILE, SR_LINE);

#define SR_OPENGL_CLEAR_ERROR() \
    while (glGetError() != GL_NO_ERROR){}


#if defined(SR_DEBUG_MODE)

#define SR_CORE_PRECONDITION(condition, message) \
    if (!(condition)) {                          \
        DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_HIGH, message, SR_FUNC, SR_FILE, SR_LINE, SR_MESSAGE_PRECONDITION); \
    }   \

#define SR_CORE_POSTCONDITION(condition, message) \
    if (!(condition)) {                          \
        DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_HIGH, message, SR_FUNC, SR_FILE, SR_LINE, SR_MESSAGE_POSTCONDITION); \
    } \

#define SR_CORE_ASSERT(condition, message) \
    if (!(condition)) {                          \
        DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_HIGH, message, SR_FUNC, SR_FILE, SR_LINE, SR_MESSAGE_ASSERTION); \
    }    \


#define SR_CORE_CHECK_CALL(x, message) \
    if (!(x)) DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_HIGH, message, SR_FUNC, SR_FILE, SR_LINE); \

#define SR_CORE_MESSAGE_ON_CONDITION(condition, message) \
    if (!condition) SR_CORE_MESSAGE(message)

#define SR_OPENGL_CALL(x) \
    SR_OPENGL_CLEAR_ERROR(); \
    x;                    \
    while (GLenum _err = glGetError()) { \
        DebugMessageHandler::pushOpenGlError(_err, #x, SR_FILE, SR_LINE); \
    }

#if defined(SR_PLATFORM_WIN64)

#define SR_CORE_HRESULT(hr) \
    DebugMessageHandler::formatHresultMessage(hr, SR_FUNC, SR_FILE, SR_LINE);

#define SR_D3D11_CALL(x) \
    do {                 \
        HRESULT _hr = x;\
        if (FAILED(_hr)){ \
            DebugMessageHandler::formatHresultMessage(_hr, #x, SR_FILE, SR_LINE);               \
        }                 \
        \
    } while (0);         \

#define SR_D3D11_CHECK_DEVICE_REMOVED(x, device) \
    do {                 \
        HRESULT _hr = x;\
        if (FAILED(_hr)){                       \
            if (_hr == DXGI_ERROR_DEVICE_REMOVED){ \
                 DebugMessageHandler::d3d11DeviceRemovedHandler(device->GetDeviceRemovedReason(), #x, SR_FILE, SR_LINE);                                    \
            }                                     \
            else{                                \
                  DebugMessageHandler::formatHresultMessage(_hr, SR_FUNC, SR_FILE, SR_LINE);                                   \
            } \
        }                 \
    } while (0);         \

#if defined(SR_COMPILER_MSVC)
#define SR_DXGI_GET_MESSAGES() DebugMessageHandler::dxgiGetMessages();
#else
#define SR_DXGI_GET_MESSAGES()
#endif

#endif

#define SR_VULKAN_CALL(x, message) \
    do {                           \
        VkResult __vkResult = x; \
        if (__vkResult != 0){ \
               DebugMessageHandler::vulkanFormatVkResultMessage(__vkResult, message, SR_FUNC, SR_FILE, SR_LINE);               \
        }\
    } while (0);                              \


#else

#define SR_CORE_PRECONDITION(condition, message)
#define SR_CORE_POSTCONDITION(condition, message)
#define SR_CORE_ASSERT(condition, message)

#define SR_CORE_CHECK_CALL(x, message) x;

#define SR_OPENGL_CALL(x) x;

#if defined(SR_PLATFORM_WIN64)

#define SR_CORE_HRESULT(hr)

#define SR_D3D11_CALL(x) x;
#define SR_D3D11_CHECK_DEVICE_REMOVED(x, device) x;

#if defined(SR_COMPILER_MSVC)
#define SR_DXGI_GET_MESSAGES()

#endif

#endif

#define SR_VULKAN_CALL(x, message) x;\

#endif
