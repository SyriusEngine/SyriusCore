#pragma once

#include "DebugMessageHandler.hpp"
#include <cassert>

#define SR_CORE_MESSAGE(message, ...) \
    DebugMessageHandler::pushMessage(SR_CORE_MESSAGE_SEVERITY_INFO, SR_CORE_MESSAGE, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE, message, ##__VA_ARGS__);

#define SR_CORE_WARNING(message, ...) \
    DebugMessageHandler::pushMessage(SR_CORE_MESSAGE_SEVERITY_MEDIUM, SR_CORE_MESSAGE, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE, message, ##__VA_ARGS__);

#define SR_CORE_EXCEPTION(message, ...) \
    DebugMessageHandler::pushMessage(SR_CORE_MESSAGE_SEVERITY_HIGH, SR_CORE_MESSAGE, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE, message, ##__VA_ARGS__);

#define SR_CORE_OPENGL_CLEAR_ERROR() \
    {                                \
        GLenum _err;                      \
        while ((_err = glGetError()) != GL_NO_ERROR) { \
            DebugMessageHandler::pushOpenGlError(_err, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE); \
        }   \
    }\


#if defined(SR_CORE_DEBUG)

#define SR_CORE_PRECONDITION(condition, message, ...) \
    if (!(condition)) {                          \
        DebugMessageHandler::pushMessage(SR_CORE_MESSAGE_SEVERITY_HIGH, SR_CORE_MESSAGE_PRECONDITION, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE, message, ##__VA_ARGS__); \
        assert(false);                                         \
    }   \

#define SR_CORE_POSTCONDITION(condition, message, ...) \
    if (!(condition)) {                          \
        DebugMessageHandler::pushMessage(SR_CORE_MESSAGE_SEVERITY_HIGH, SR_CORE_MESSAGE_POSTCONDITION, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE, message, ##__VA_ARGS__); \
        assert(false); \
    } \

#define SR_CORE_ASSERT(condition, message, ...) \
    if (!(condition)) {                          \
        DebugMessageHandler::pushMessage(SR_CORE_MESSAGE_SEVERITY_HIGH, SR_CORE_MESSAGE_ASSERTION, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE, message, ##__VA_ARGS__); \
    }    \


#define SR_CORE_CHECK_CALL(x, message, ...) \
    if (!(x)) DebugMessageHandler::pushMessage(SR_CORE_MESSAGE_SEVERITY_MEDIUM, SR_CORE_MESSAGE, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE, message, ##__VA_ARGS__); \

#define SR_CORE_MESSAGE_ON_CONDITION(condition, message) \
    if (!condition) SR_CORE_MESSAGE(message)

#define SR_CORE_OPENGL_CALL(x) \
    SR_CORE_OPENGL_CLEAR_ERROR(); \
    x;                    \
    GLenum _err;                      \
    while ((_err = glGetError()) != GL_NO_ERROR) { \
        DebugMessageHandler::pushOpenGlError(_err, #x, SR_CORE_FILE, SR_CORE_LINE); \
    }

#if defined(SR_CORE_PLATFORM_WIN64)

#define SR_CORE_HRESULT(hr) \
    DebugMessageHandler::formatHresultMessage(hr, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE);

#define SR_CORE_D3D11_CHECK_DEVICE_REMOVED(x, device) \
    do {                 \
        HRESULT _hr = x;\
        if (FAILED(_hr)){                       \
            if (_hr == DXGI_ERROR_DEVICE_REMOVED){ \
                 DebugMessageHandler::d3d11DeviceRemovedHandler((device)->GetDeviceRemovedReason(), #x, SR_CORE_FILE, SR_CORE_LINE);                                    \
            }                                     \
            else{                                \
                  DebugMessageHandler::formatHresultMessage(_hr, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE);                                   \
            } \
        }                 \
    } while (0);         \

#if defined(SR_COMPILER_MSVC)
#define SR_CORE_DXGI_GET_MESSAGES() DebugMessageHandler::dxgiGetMessages();

#define SR_CORE_D3D11_CALL(x) \
    do {                 \
        HRESULT _hr = x;\
        if (FAILED(_hr)){ \
            DebugMessageHandler::formatHresultMessage(_hr, #x, SR_CORE_FILE, SR_CORE_LINE); \
            DebugMessageHandler::dxgiGetMessages();     \
        }                 \
        \
    } while (0);         \

#else
#define SR_CORE_DXGI_GET_MESSAGES()

#define SR_CORE_D3D11_CALL(x) \
    do {                 \
        HRESULT _hr = x;\
        if (FAILED(_hr)){ \
            DebugMessageHandler::formatHresultMessage(_hr, #x, SR_CORE_FILE, SR_CORE_LINE);               \
        }                 \
        \
    } while (0);         \

#endif

#endif

#define SR_VULKAN_CALL(x, message) \
    do {                           \
        VkResult __vkResult = x; \
        if (__vkResult != 0){ \
               DebugMessageHandler::vulkanFormatVkResultMessage(__vkResult, message, SR_CORE_FUNC, SR_CORE_FILE, SR_CORE_LINE);               \
        }\
    } while (0);                              \


#else

#define SR_CORE_PRECONDITION(condition, message)
#define SR_CORE_POSTCONDITION(condition, message)
#define SR_CORE_ASSERT(condition, message)

#define SR_CORE_CHECK_CALL(x, message) x;

#define SR_CORE_OPENGL_CALL(x) x;

#if defined(SR_CORE_PLATFORM_WIN64)

#define SR_CORE_HRESULT(hr)

#define SR_CORE_D3D11_CALL(x) x;
#define SR_CORE_D3D11_CHECK_DEVICE_REMOVED(x, device) x;

#if defined(SR_CORE_COMPILER_MSVC)
#define SR_CORE_DXGI_GET_MESSAGES()

#endif

#endif

#define SR_VULKAN_CALL(x, message) x;\

#endif
