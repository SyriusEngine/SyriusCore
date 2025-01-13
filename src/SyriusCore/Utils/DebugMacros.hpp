#pragma once

#include "CoreLogger.hpp"

#if defined(SR_DEBUG)
    #define SR_CORE_OPENGL_CLEAR_ERROR() CoreLogger::openGLClearError(SR_FUNC, SR_FILE, SR_LINE);

    #define SR_CORE_OPENGL_CALL(x) \
        CoreLogger::openGLClearErrorNoLog(); \
        x; \
        CoreLogger::openGLClearError(#x, SR_FILE, SR_LINE);

    #if defined(SR_PLATFORM_WIN64)
        #define SR_CORE_HRESULT(hr) CoreLogger::formatHresultMessage(hr, SR_FUNC, SR_FILE, SR_LINE);
        
        #if defined(SR_COMPILER_MSVC)
            #define SR_CORE_DXGI_GET_MESSAGES() CoreLogger::dxgiGetMessages();
            #define SR_CORE_D3D11_CALL(x) \
                do { \
                    HRESULT _hr = x;\
                    if (FAILED(_hr)){ \
                        CoreLogger::formatHresultMessage(_hr, #x, SR_FILE, SR_LINE); \
                        CoreLogger::dxgiGetMessages(); \
                    } \
                } while (0);

            #define SR_CORE_DXGI_CHECK_DEVICE_REMOVED(x, device) \
                do { \
                    HRESULT _hr = x; \
                    if (FAILED(_hr)){                            \
                        CoreLogger::dxgiGetMessages(); \
                        if (_hr == DXGI_ERROR_DEVICE_REMOVED){ \
                            CoreLogger::dxgiLogDeviceRemoved((device)->GetDeviceRemovedReason(), #x, SR_FILE, SR_LINE); \
                        } \
                        else{ \
                            CoreLogger::formatHresultMessage(_hr, SR_FUNC, SR_FILE, SR_LINE); \
                        } \
                    } \
                } while (0);
        #else
            #define SR_CORE_DXGI_GET_MESSAGES()
            #define SR_CORE_D3D11_CALL(x) \
                do { \
                    HRESULT _hr = x;\
                    if (FAILED(_hr)){ \
                        CoreLogger::formatHresultMessage(_hr, #x, SR_FILE, SR_LINE); \
                    } \
                } while (0);

            #define SR_CORE_DXGI_CHECK_DEVICE_REMOVED(x, device) \
            do { \
                HRESULT _hr = x; \
                if (FAILED(_hr)){ \
                    if (_hr == DXGI_ERROR_DEVICE_REMOVED){ \
                        CoreLogger::dxgiLogDeviceRemoved((device)->GetDeviceRemovedReason(), #x, SR_FILE, SR_LINE); \
                    } \
                    else{ \
                        CoreLogger::formatHresultMessage(_hr, SR_FUNC, SR_FILE, SR_LINE); \
                    } \
                } \
            } while (0);
        #endif
#endif

#else
    #define SR_CORE_OPENGL_CLEAR_ERROR()
    #define SR_CORE_OPENGL_CALL(x) x;

    #if defined(SR_PLATFORM_WIN64)
        #define SR_CORE_HRESULT(hr)
        #define SR_CORE_DXGI_CHECK_DEVICE_REMOVED(x, device) x;
        #define SR_CORE_D3D11_CALL(x) x;
        #define SR_CORE_DXGI_GET_MESSAGES()
        #define SR_CORE_D3D11_CALL(x) x;
    #endif

#endif

