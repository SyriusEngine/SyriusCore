#include "PlatformAPIWin32Impl.hpp"
#include "../Window/SyriusWindowWin32Impl.hpp"

#if defined(SR_CORE_PLATFORM_WIN64)

namespace Syrius{

    VulkanPlatformDescWin32::VulkanPlatformDescWin32()
    : VulkanPlatformDesc() {
        addExtension("VK_KHR_surface");
        addExtension("VK_KHR_win32_surface");
    }

    PlatformAPIWin32Impl::PlatformAPIWin32Impl()
    : PlatformAPI(VulkanPlatformDescWin32()){
        setProcessDpiAware();
        registerWindowClass();
    }

    PlatformAPIWin32Impl::~PlatformAPIWin32Impl() {
        unregisterWindowClass();
    }

    uint32 PlatformAPIWin32Impl::getPrimaryScreenWidth() {
        HDC screenDC = GetDC(nullptr);
        uint32_t width = GetDeviceCaps(screenDC, HORZRES);
        ReleaseDC(nullptr, screenDC);
        return width;
    }

    uint32 PlatformAPIWin32Impl::getPrimaryScreenHeight() {
        HDC screenDC = GetDC(nullptr);
        uint32_t height = GetDeviceCaps(screenDC, VERTRES);
        ReleaseDC(nullptr, screenDC);
        return height;
    }

    void PlatformAPIWin32Impl::initPlatformGlad(HDC hdc) {
        if (!m_PlatformGladInstances){
            m_GlVersion = gladLoaderLoadWGL(hdc);
            SR_CORE_ASSERT(m_GlVersion > 0, "Failed to initialize WGL");
        }
        m_PlatformGladInstances++;
    }

    void PlatformAPIWin32Impl::terminatePlatformGlad() {
        m_PlatformGladInstances--;
    }

    Resource<SyriusWindow> PlatformAPIWin32Impl::createWindow(const WindowDesc &windowDesc) {
        return Resource<SyriusWindow>(new SyriusWindowWin32Impl(windowDesc, this));
    }

    void PlatformAPIWin32Impl::setProcessDpiAware() {
        HINSTANCE shCoreDll = LoadLibraryW(L"Shcore.dll");
        if (shCoreDll){
            enum ProcessDpiAwareness
            {
                ProcessDpiUnaware         = 0,
                ProcessSystemDpiAware     = 1,
                ProcessPerMonitorDpiAware = 2
            };
            typedef HRESULT (WINAPI* SetProcessDpiAwarenessFuncType)(ProcessDpiAwareness);
            auto setProcessDpiAwarenessFunc = reinterpret_cast<SetProcessDpiAwarenessFuncType>(GetProcAddress(shCoreDll, "SetProcessDpiAwareness"));
            if (setProcessDpiAwarenessFunc){
                if (setProcessDpiAwarenessFunc(ProcessSystemDpiAware) == E_INVALIDARG){
                    SR_CORE_WARNING("Failed to set process DPI awareness using shCore.dll libary, falling back on user32.dll");
                }
                else{
                    FreeLibrary(shCoreDll);
                    return;
                }
            }
            FreeLibrary(shCoreDll);

        }
        // when setting DPI awareness using shcore.dll failed, fall back and use user32.dll and try again
        HINSTANCE user32Dll = LoadLibraryW(L"user32.dll");
        if (user32Dll){
            typedef BOOL (WINAPI* SetProcessDPIAwareFuncType)(void);
            auto setProcessDPIAwareFunc = reinterpret_cast<SetProcessDPIAwareFuncType>(GetProcAddress(user32Dll, "SetProcessDPIAware"));
            if (setProcessDPIAwareFunc){
                if (!setProcessDPIAwareFunc()){
                    SR_CORE_WARNING("Failed to set process DPI awareness");
                }
            }
            FreeLibrary(user32Dll);

        }
    }

    void PlatformAPIWin32Impl::registerWindowClass() {
        WNDCLASSEXW wndClass;
        wndClass.cbSize = sizeof(WNDCLASSEXW);
        wndClass.style = 0;
        wndClass.lpfnWndProc = &SyriusWindowWin32Impl::windowEventProc;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = GetModuleHandleW(nullptr);
        wndClass.hIcon = nullptr;
        wndClass.hCursor = nullptr;
        wndClass.hbrBackground = 0;
        wndClass.lpszMenuName = L"SYRIUS_CORE";
        wndClass.lpszClassName = s_SyriusWindowClass;
        wndClass.hIconSm = nullptr;

        RegisterClassExW(&wndClass);

        SR_CORE_HRESULT(GetLastError());
    }

    void PlatformAPIWin32Impl::unregisterWindowClass() {
        UnregisterClassW(s_SyriusWindowClass, GetModuleHandleW(nullptr));
    }

}

#endif
