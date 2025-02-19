#include "CoreLogger.hpp"

#include <sstream>

namespace Syrius{
    
#if defined(SR_COMPILER_MSVC)
    u64 CoreLogger::m_DxgiMessageIndex = 0;
    IDXGIInfoQueue* CoreLogger::m_DxgiInfoQueue = nullptr;
#endif

    void CoreLogger::openGLMessageCallback(GLenum source, u32 type, u32 id, u32 severity, i32 length, const char* message, const void* userParam) {
        Message msg;
        msg.message = "Error Group = ";
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:               msg.message += "Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: msg.message += "Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  msg.message += "Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         msg.message += "Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         msg.message += "Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              msg.message += "Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          msg.message += "Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           msg.message += "Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               msg.message += "Other"; break;
            default:                                msg.message += "UNKNOWN TYPE"; break;
        }
        msg.message += " (" + std::to_string(type) + ")\nsource = ";
        switch (source) {
            case GL_DEBUG_SOURCE_API:               msg.message += "API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     msg.message += "Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   msg.message += "Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:       msg.message += "Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:       msg.message += "Application"; break;
            case GL_DEBUG_SOURCE_OTHER:             msg.message += "Other"; break;
            default:                                msg.message += "UNKNOWN SOURCE"; break;
        }
        msg.message += " (" + std::to_string(source) + ")\n";
        msg.message += "Message length = " + std::to_string(length) + "\n";
        msg.message += "ID = " + std::to_string(id) + "\n";
        msg.message += "Message = " + std::string(message) + "\n";
        msg.source = "OpenGLCallback";
        switch (severity) {
            case GL_DEBUG_SEVERITY_LOW:             msg.severity = SR_MESSAGE_SEVERITY_LOW; break;
            case GL_DEBUG_SEVERITY_MEDIUM:          msg.severity = SR_MESSAGE_SEVERITY_MEDIUM; break;
            case GL_DEBUG_SEVERITY_HIGH:            msg.severity = SR_MESSAGE_SEVERITY_HIGH; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:    msg.severity = SR_MESSAGE_SEVERITY_INFO; break;
            default:                                msg.severity = SR_MESSAGE_SEVERITY_INFO; break;
        }

        Logger::logMessage(msg);
    }

    void CoreLogger::openGLError(GLenum error, const std::string &function, const std::string &file, u32 line) {
        Message msg;
        msg.message = "OpenGL encountered a state error = ";
        switch (error) {
            case GL_INVALID_ENUM:                   msg.message += "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                  msg.message += "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:              msg.message += "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                 msg.message += "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:                msg.message += "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                  msg.message += "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  msg.message += "INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_CONTEXT_LOST:                   msg.message += "CONTEXT_LOST"; break;
            case GL_TABLE_TOO_LARGE:                msg.message += "TABLE_TOO_LARGE"; break;
            default:                                msg.message += "UNKNOWN ERROR"; break;
        }
        msg.message += " (" + std::to_string(error) + ")";
        msg.source = "OpenGL";
        msg.severity = SR_MESSAGE_SEVERITY_HIGH;
        msg.function = function;
        msg.file = file;
        msg.line = line;
        Logger::logMessage(msg);
    }

    void CoreLogger::openGLClearError(const std::string& function, const std::string& file, u32 line) {
        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            openGLError(error, function, file, line);
        }
    }

    void CoreLogger::openGLClearErrorNoLog() {
        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {}
    }

#if defined(SR_PLATFORM_WIN64)
    void CoreLogger::formatHresultMessage(HRESULT hr, const std::string& function, const std::string& file, u32 line){
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
            Message msg;
            msg.message = "Code = " + std::to_string(hr) + ", message = " + std::string(errorText);

            LocalFree(errorText);
            errorText = nullptr;
            msg.severity = SR_MESSAGE_SEVERITY_UNKNOWN;
            msg.source = "HRESULT";
            msg.function = function;
            msg.file = file;
            msg.line = line;
            Logger::logMessage(msg);
        }
    }

    void CoreLogger::dxgiLogDeviceRemoved(HRESULT hr, const std::string &function, const std::string &file, u32 line) {
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
            Message msg;
            msg.message = "The application has thrown a DXGI_DEVICE_REMOVED exception. This indicates a driver crash"
                              "(or less likely that the graphics device is pulled away from the system). Detailed info: "
                              "Code = " + std::to_string(hr) + " msg = " + std::string(errorText);

            LocalFree(errorText);
            errorText = nullptr;
            msg.source = "DXGI";
            msg.severity = SR_MESSAGE_SEVERITY_FATAL; //DXGI_ERROR_DEVICE_REMOVED is always fatal
            msg.function = function;
            msg.file = file;
            msg.line = line;
            Logger::logMessage(msg);
        }
    }

#if defined(SR_COMPILER_MSVC)
    SR_MESSAGE_SEVERITY getSrMessageSeverity(DXGI_INFO_QUEUE_MESSAGE_SEVERITY severity){
        switch (severity) {
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:         return SR_MESSAGE_SEVERITY_INFO;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:      return SR_MESSAGE_SEVERITY_LOW;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:      return SR_MESSAGE_SEVERITY_MEDIUM;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:        return SR_MESSAGE_SEVERITY_HIGH;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:   return SR_MESSAGE_SEVERITY_FATAL;
            default:                                            return SR_MESSAGE_SEVERITY_INFO;
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

    void CoreLogger::dxgiGetMessages(){
        static bool libLoaded = false;
        if (!libLoaded) {
            typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);
            const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
            if (hModDxgiDebug){
                const auto dxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"));
                if (dxgiGetDebugInterface){
                    HRESULT hr = dxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&m_DxgiInfoQueue));
                    libLoaded = true;
                    formatHresultMessage(hr, "DXGIGetDebugInterface", SR_FILE, SR_LINE);
                }
            }
        }

        const auto end = m_DxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
        for (u64 i = m_DxgiMessageIndex; i < end; i++){
            Size messageLength = 0;
            m_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, reinterpret_cast<SIZE_T *>(&messageLength));
            std::vector<byte> buffer(messageLength);
            auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(&buffer[0]);
            m_DxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, reinterpret_cast<SIZE_T *>(&messageLength));

            Message msg;
            msg.source = "DXGI";
            msg.severity = getSrMessageSeverity(message->Severity);
            msg.message = "Code = " + std::to_string(message->ID) + ", Category = " + getDxgiCategoryAsString(message->Category) + ",\n message = " + std::string(message->pDescription);;
            msg.function = "DXGI_DEBUG_ALL";
            msg.file = "";
            msg.line = 0;
            Logger::logMessage(msg);
        }
        m_DxgiMessageIndex = end;
    }
#endif

#else
    void CoreLogger::glfwCallback(const int code, const char *description) {
        std::stringstream ss;
        ss << "Error = ";
        switch (code) {
            case GLFW_NO_ERROR:             ss << "GLFW_NO_ERROR";              break;
            case GLFW_NOT_INITIALIZED:      ss << "GLFW_NOT_INITIALIZED";       break;
            case GLFW_NO_CURRENT_CONTEXT:   ss << "GLFW_NO_CURRENT_CONTEXT";    break;
            case GLFW_INVALID_ENUM:         ss << "GLFW_INVALID_ENUM";          break;
            case GLFW_INVALID_VALUE:        ss << "GLFW_INVALID_VALUE";         break;
            case GLFW_OUT_OF_MEMORY:        ss << "GLFW_OUT_OF_MEMORY";         break;
            case GLFW_API_UNAVAILABLE:      ss << "GLFW_API_UNAVAILABLE";       break;
            case GLFW_VERSION_UNAVAILABLE:  ss << "GLFW_VERSION_UNAVAILABLE";   break;
            case GLFW_PLATFORM_ERROR:       ss << "GLFW_PLATFORM_ERROR";        break;
            case GLFW_FORMAT_UNAVAILABLE:   ss << "GLFW_FORMAT_UNAVAILABLE";    break;
            case GLFW_NO_WINDOW_CONTEXT:    ss << "GLFW_NO_WINDOW_CONTEXT";     break;
            default:                        ss << "GLFW_UNKNOWN_ERROR";         break;
        }
        ss << " Code = " << code << " (0x" << std::hex << std::to_string(code) << ") ";
        ss << "Description = " << description;

        Message msg;
        msg.source = "GLFW";
        msg.message = ss.str();
        msg.function = "glfwCallback";
        msg.line = 0;
        Logger::logMessage(msg);
    }



#endif

}