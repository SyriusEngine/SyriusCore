#pragma once

#include "DebugMessageHandler.hpp"

#define SR_CORE_MESSAGE(message) \
    DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_INFO, message, SR_FUNC, SR_FILE, SR_LINE);

#define SR_CORE_WARNING(message) \
    DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_MEDIUM, message, SR_FUNC, SR_FILE, SR_LINE);

#define SR_CORE_EXCEPTION(message) \
    DebugMessageHandler::pushMessage(SR_MESSAGE_SEVERITY_HIGH, message, SR_FUNC, SR_FILE, SR_LINE);

#define SR_CORE_HRESULT(hr) \
    DebugMessageHandler::formatHresultMessage(hr, SR_FUNC, SR_FILE, SR_LINE);

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

#define SR_OPENGL_CHECK_CALL(x) \
    SR_OPENGL_CLEAR_ERROR(); \
    x;                    \
    while (GLenum __err = glGetError()) { \
        DebugMessageHandler::pushOpenGlError(__err, #x, SR_FILE, SR_LINE); \
    }

#define SR_CORE_CHECK_HRESULT(hr) \
    if (FAILED(hr)) DebugMessageHandler::formatHresultMessage(hr, SR_FUNC, SR_FILE, SR_LINE);

#define SR_CORE_CHECK_D3D11_DEVICE_REMOVED(hr) \
    if (FAILED(hr)) DebugMessageHandler::d3d11DeviceRemovedHandler(hr, SR_FUNC, SR_FILE, SR_LINE);

#define SR_VULKAN_CALL(x, message) \
    do {                           \
        int32 __vkResult = x; \
        if (__vkResult != 0){ \
               DebugMessageHandler::vulkanFormatVkResultMessage(__vkResult, message, SR_FUNC, SR_FILE, SR_LINE);               \
        }\
    } while (0);                              \


#else

#define SR_CORE_PRECONDITION(condition, message)
#define SR_CORE_POSTCONDITION(condition, message)
#define SR_CORE_ASSERT(condition, message)

#define SR_CORE_CHECK_CALL(x, message) x;

#define SR_OPENGL_CHECK_CALL(x) x;
#define SR_CORE_CHECK_HRESULT(hr) hr;
#define SR_CORE_CHECK_D3D11_DEVICE_REMOVED(hr) hr;

#define SR_VULKAN_CALL(x, message) x;\

#endif
