#include "DebugMessageHandler.hpp"

namespace Syrius{

    void defaultMessageHandler(const Message& msg){
        std::string message = "[Syrius: " + msg.function + "]: severity = " + getMessageSeverityString(msg.severity) + ", type = " +
                              getMessageTypeString(msg.messageType) + "\n";
        message += "File = " + msg.file + ", line = " + std::to_string(msg.line) + "\n";
        message += "Message = " + msg.message + "\n";
        std::cerr << message;
        std::cerr << "\n\n--------------------------------------------------------------------------------\n";
    }

    handleDebugMessageFunc DebugMessageHandler::m_MessageHandler = defaultMessageHandler;
#if defined(SR_COMPILER_MSVC)
    uint64 DebugMessageHandler::m_DxgiMessageIndex = 0;
    IDXGIInfoQueue* DebugMessageHandler::m_DxgiInfoQueue = nullptr;
#endif

    void DebugMessageHandler::setDebugMessageHandler(handleDebugMessageFunc newHandler) {
        m_MessageHandler = newHandler;
    }

    void DebugMessageHandler::pushOpenGLMessageCallback(GLenum source, uint32 type, uint32 id, uint32 severity, int32 length, const char* message, const void* userParam) {
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
        SR_CORE_MESSAGE_SEVERITY sev;
        switch (severity) {
            case GL_DEBUG_SEVERITY_LOW:             sev = SR_CORE_MESSAGE_SEVERITY_LOW; break;
            case GL_DEBUG_SEVERITY_MEDIUM:          sev = SR_CORE_MESSAGE_SEVERITY_MEDIUM; break;
            case GL_DEBUG_SEVERITY_HIGH:            sev = SR_CORE_MESSAGE_SEVERITY_HIGH; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:    sev = SR_CORE_MESSAGE_SEVERITY_INFO; break;
            default:                                sev = SR_CORE_MESSAGE_SEVERITY_INFO; break;
        }

        pushMessage(sev, SR_CORE_MESSAGE_OPENGL, "OpenGLCallback", "", 0, msg);
    }

    void DebugMessageHandler::pushOpenGlError(GLenum error, const std::string &function, const std::string &file,
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
        pushMessage(SR_CORE_MESSAGE_SEVERITY_HIGH, SR_CORE_MESSAGE_OPENGL, function, file, line, msg);
    }

#if defined(SR_CORE_PLATFORM_WIN64)
    void DebugMessageHandler::formatHresultMessage(HRESULT hr, const std::string& function, const std::string& file, uint32 line){
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

            LocalFree(errorText);
            errorText = nullptr;

            pushMessage(SR_CORE_MESSAGE_SEVERITY_HIGH, SR_CORE_MESSAGE_HRESULT, function, file, line, msg);
        }
    }

    void
    DebugMessageHandler::d3d11DeviceRemovedHandler(HRESULT hr, const std::string &function, const std::string &file,
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

            LocalFree(errorText);
            errorText = nullptr;

            pushMessage(SR_CORE_MESSAGE_SEVERITY_HIGH, SR_CORE_MESSAGE_HRESULT, function, file, line, msg);

        }
    }

#if defined(SR_COMPILER_MSVC)
    SR_CORE_MESSAGE_SEVERITY getSrMessageSeverity(DXGI_INFO_QUEUE_MESSAGE_SEVERITY severity){
        switch (severity) {
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:         return SR_CORE_MESSAGE_SEVERITY_INFO;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:      return SR_CORE_MESSAGE_SEVERITY_LOW;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:      return SR_CORE_MESSAGE_SEVERITY_MEDIUM;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:        return SR_CORE_MESSAGE_SEVERITY_HIGH;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:   return SR_CORE_MESSAGE_SEVERITY_HIGH;
            default:                                            return SR_CORE_MESSAGE_SEVERITY_HIGH;
        }
    }

    std::string getDxgiCategoryAsString(DXGI_INFO_QUEUE_MESSAGE_CATEGORY category){
        switch (category){
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN:                  return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_MISCELLANEOUS:            return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_MISCELLANEOUS";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_INITIALIZATION:           return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_INITIALIZATION";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_CLEANUP:                  return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_CLEANUP";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_COMPILATION:              return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_COMPILATION";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_CREATION:           return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_CREATION";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_SETTING:            return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_SETTING";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_GETTING:            return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_GETTING";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:    return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_RESOURCE_MANIPULATION";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_EXECUTION:                return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_EXECUTION";
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_SHADER:                   return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_SHADER";
            default: {
                return "DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN";
            }
        }
    }

    void DebugMessageHandler::dxgiGetMessages(){
        static bool libLoaded = false;
        if (!libLoaded) {
            typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);
            const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
            if (hModDxgiDebug){
                const auto dxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"));
                if (dxgiGetDebugInterface){
                    HRESULT hr = dxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&m_DxgiInfoQueue));
                    libLoaded = true;
                    formatHresultMessage(hr, "DXGIGetDebugInterface", SR_CORE_FILE, SR_CORE_LINE);
                }
            }
        }

        const auto end = m_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
        for (uint64 i = m_DxgiMessageIndex; i < end; i++){
            Size messageLength = 0;
            m_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, reinterpret_cast<SIZE_T *>(&messageLength));
            std::vector<byte> buffer(messageLength);
            auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(&buffer[0]);
            m_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, reinterpret_cast<SIZE_T *>(&messageLength));

            Message msgStruct;
            msgStruct.type = SR_CORE_MESSAGE_DXGI;
            msgStruct.severity = getSrMessageSeverity(message->Severity);
            msgStruct.message = "Code = " + std::to_string(message->ID) + ", Category = " + getDxgiCategoryAsString(message->Category) + ",\n message = " + std::string(message->pDescription);;
            msgStruct.function = "DXGI_DEBUG_ALL";
            msgStruct.file = "";
            msgStruct.line = 0;
            m_MessageHandler(msgStruct);

        }
        m_DxgiMessageIndex = end;
    }
#endif

#elif defined(SR_CORE_PLATFORM_LINUX)
    int32 DebugMessageHandler::x11ErrorHandler(Display* display, XErrorEvent* event){
        char buffer[1024];
        XGetErrorText(display, event->error_code, buffer, sizeof(buffer));
        Message msgStruct;
        msgStruct.type = SR_CORE_MESSAGE_X11;
        msgStruct.severity = SR_CORE_MESSAGE_SEVERITY_MEDIUM;
        msgStruct.message = buffer;
        msgStruct.function = "X11";
        msgStruct.file = "";
        msgStruct.line = 0;
        m_MessageHandler(msgStruct);
    }

#endif

    void DebugMessageHandler::vulkanFormatVkResultMessage(VkResult result, const std::string &message,
                                                          const std::string &function, const std::string &file,
                                                          uint32 line) {

        std::string msg = "Vulkan function returned exit code = " + std::to_string(result) + "(";
        SR_CORE_MESSAGE_SEVERITY severity = SR_CORE_MESSAGE_SEVERITY_HIGH;
        switch (result) {
            case VK_SUCCESS:                                    msg += "VK_SUCCESS"; severity = SR_CORE_MESSAGE_SEVERITY_INFO; break;
            case VK_NOT_READY:                                  msg += "VK_NOT_READY"; break;
            case VK_TIMEOUT:                                    msg += "VK_TIMEOUT"; break;
            case VK_EVENT_SET:                                  msg += "VK_EVENT_SET"; break;
            case VK_EVENT_RESET:                                msg += "VK_EVENT_RESET"; break;
            case VK_INCOMPLETE:                                 msg += "VK_INCOMPLETE"; break;
            case VK_ERROR_OUT_OF_HOST_MEMORY:                   msg += "VK_ERROR_OUT_OF_HOST_MEMORY"; break;
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:                 msg += "VK_ERROR_OUT_OF_DEVICE_MEMORY"; break;
            case VK_ERROR_INITIALIZATION_FAILED:                msg += "VK_ERROR_INITIALIZATION_FAILED"; break;
            case VK_ERROR_DEVICE_LOST:                          msg += "VK_ERROR_DEVICE_LOST"; break;
            case VK_ERROR_MEMORY_MAP_FAILED:                    msg += "VK_ERROR_MEMORY_MAP_FAILED"; break;
            case VK_ERROR_LAYER_NOT_PRESENT:                    msg += "VK_ERROR_LAYER_NOT_PRESENT"; break;
            case VK_ERROR_EXTENSION_NOT_PRESENT:                msg += "VK_ERROR_EXTENSION_NOT_PRESENT"; break;
            case VK_ERROR_FEATURE_NOT_PRESENT:                  msg += "VK_ERROR_FEATURE_NOT_PRESENT"; break;
            case VK_ERROR_INCOMPATIBLE_DRIVER:                  msg += "VK_ERROR_INCOMPATIBLE_DRIVER"; break;
            case VK_ERROR_TOO_MANY_OBJECTS:                     msg += "VK_ERROR_TOO_MANY_OBJECTS"; break;
            case VK_ERROR_FORMAT_NOT_SUPPORTED:                 msg += "VK_ERROR_FORMAT_NOT_SUPPORTED"; break;
            case VK_ERROR_FRAGMENTED_POOL:                      msg += "VK_ERROR_FRAGMENTED_POOL"; break;
            case VK_ERROR_UNKNOWN:                              msg += "VK_ERROR_UNKNOWN"; break;
            case VK_ERROR_OUT_OF_POOL_MEMORY:                   msg += "VK_ERROR_OUT_OF_POOL_MEMORY"; break;
            case VK_ERROR_INVALID_EXTERNAL_HANDLE:              msg += "VK_ERROR_INVALID_EXTERNAL_HANDLE"; break;
            case VK_ERROR_FRAGMENTATION:                        msg += "VK_ERROR_FRAGMENTATION"; break;
            case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:       msg += "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS"; break;
            case VK_PIPELINE_COMPILE_REQUIRED:                  msg += "VK_PIPELINE_COMPILE_REQUIRED"; break;
            case VK_ERROR_SURFACE_LOST_KHR:                     msg += "VK_ERROR_SURFACE_LOST_KHR"; break;
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:             msg += "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"; break;
            case VK_SUBOPTIMAL_KHR:                             msg += "VK_SUBOPTIMAL_KHR"; break;
            case VK_ERROR_OUT_OF_DATE_KHR:                      msg += "VK_ERROR_OUT_OF_DATE_KHR"; break;
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:             msg += "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR"; break;
            case VK_ERROR_VALIDATION_FAILED_EXT:                msg += "VK_ERROR_VALIDATION_FAILED_EXT"; break;
            case VK_ERROR_INVALID_SHADER_NV:                    msg += "VK_ERROR_INVALID_SHADER_NV"; break;
            case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: msg += "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT"; break;
            case VK_ERROR_NOT_PERMITTED_KHR:                    msg += "VK_ERROR_NOT_PERMITTED_KHR"; break;
            case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:  msg += "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT"; break;
            case VK_THREAD_IDLE_KHR:                            msg += "VK_THREAD_IDLE_KHR"; break;
            case VK_THREAD_DONE_KHR:                            msg += "VK_THREAD_DONE_KHR"; break;
            case VK_OPERATION_DEFERRED_KHR:                     msg += "VK_OPERATION_DEFERRED_KHR"; break;
            case VK_OPERATION_NOT_DEFERRED_KHR:                 msg += "VK_OPERATION_NOT_DEFERRED_KHR"; break;
            case VK_RESULT_MAX_ENUM:                            msg += "VK_RESULT_MAX_ENUM"; break;
        }
        msg += ")\n extra info = " + message;

        pushMessage(severity, SR_CORE_MESSAGE_VULKAN, function, file, line, msg);

    }

    VkBool32 DebugMessageHandler::vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                void *pUserData) {
        SR_CORE_MESSAGE_SEVERITY severity;
        switch (messageSeverity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:           severity = SR_CORE_MESSAGE_SEVERITY_INFO; break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:              severity = SR_CORE_MESSAGE_SEVERITY_INFO; break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:           severity = SR_CORE_MESSAGE_SEVERITY_MEDIUM; break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:             severity = SR_CORE_MESSAGE_SEVERITY_HIGH; break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:    severity = SR_CORE_MESSAGE_SEVERITY_LOW; break;
        }

        std::string msgType;
        switch (messageType) {
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:       msgType = "GENERAL"; break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:   msgType = "PERFORMANCE"; break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:    msgType = "VALIDATION"; break;
            default:                                                msgType = ""; break;
        }

        std::string msg = "[Vulkan message callback]: type = " + msgType;
        msg += " Message = " + std::string(pCallbackData->pMessage);

        pushMessage(severity, SR_CORE_MESSAGE_VULKAN, "Vulkan callback", "", 0, msg);

        return VK_FALSE;
    }


}