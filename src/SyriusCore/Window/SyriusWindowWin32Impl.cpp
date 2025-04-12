#include "SyriusWindowWin32Impl.hpp"
#include "../Context/OpenGL/Internal/WglContext.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    u32 SyriusWindowWin32Impl::m_WindowCount = 0;

    SyriusWindowWin32Impl::SyriusWindowWin32Impl(const WindowDesc &desc):
    SyriusWindow(desc),
    m_Hwnd(nullptr),
    m_Callback(0),
    m_Icon(nullptr),
    m_CaptureMouseAndKeyboardEvents(true) {
        if (m_WindowCount == 0){
            registerWindowClass();
            setProcessDPIAwareness();
        }

        DWORD winStyles = decodeWindowStyle(desc.style);
        DWORD exWinStyles = decodeExtendedWindowStyle(desc.style);

        // current window info (used when enabling fullscreen for this window)
        RECT rect = {0, 0, static_cast<LONG>(desc.width), static_cast<LONG>(desc.height)};
        m_SavedWindowInfo.m_Rect = rect;
        m_SavedWindowInfo.m_Style = winStyles;
        m_SavedWindowInfo.m_ExStyle = exWinStyles;
        m_SavedWindowInfo.m_IsMaximized = false;

        auto wTitle = stringToWideString(desc.title);
        m_Hwnd = CreateWindowExW(exWinStyles,
                                 s_SyriusWindowClass,
                                 wTitle.c_str(),
                                 winStyles,
                                 desc.xPos, desc.yPos,
                                 desc.width, desc.height,
                                 nullptr, nullptr,
                                 GetModuleHandleW(nullptr),
                                 this);
        if (!m_Hwnd){
            SR_LOG_WARNING("SyriusWindowWin32Impl", "Failed to create window");
        }
        m_WindowCount++;
        ShowWindow(m_Hwnd, SW_SHOW);
        m_Open = true;
        m_Focused = true;

        // store a pointer to our Window on the Win32 API side
        SetWindowLongPtrW(m_Hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        m_Callback = SetWindowLongPtrW(m_Hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&SyriusWindowWin32Impl::windowEventProc));

        //register raw input devices
        RAWINPUTDEVICE mouse;
        mouse.usUsagePage = 0x01; //mouse
        mouse.usUsage = 0x02;     //mouse
        mouse.dwFlags = 0;
        mouse.hwndTarget = nullptr; // register to all windows, maybe change this later to only register to this window

        RAWINPUTDEVICE keyboard;
        keyboard.usUsagePage = 0x01; //keyboard
        keyboard.usUsage = 0x06;     //keyboard
        keyboard.dwFlags = 0;
        keyboard.hwndTarget = nullptr; // register to all windows, maybe change this later to only register to this window

        SR_LOG_WARNING_IF_FALSE(RegisterRawInputDevices(&mouse, 1, sizeof(mouse)), "SyriusWindowWin32Impl", "Failed to register raw input mouse");
        SR_LOG_WARNING_IF_FALSE(RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard)), "SyriusWindowWin32Impl", "Failed to register raw input keyboard");
    }

    SyriusWindowWin32Impl::~SyriusWindowWin32Impl() {
      	if (m_UseImGui){
            destroyImGuiContext();
        }
        if (m_Hwnd){
            DestroyWindow(m_Hwnd);
            m_Hwnd = nullptr;
            m_WindowCount--;
        }
        if (m_WindowCount == 0){
            unregisterWindowClass();
        }
    }

    void SyriusWindowWin32Impl::close() {
        m_Open = false;
        ShowWindow(m_Hwnd, SW_HIDE);
    }

    void SyriusWindowWin32Impl::setPosition(i32 posX, i32 posY) {
        if (!m_Fullscreen){
            m_PosX = posX;
            m_PosY = posY;

            SR_LOG_WARNING_IF_FALSE(SetWindowPos(m_Hwnd, nullptr, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER), "SyriusWindowWin32Impl", "Failed to set window position");
        }
    }

    void SyriusWindowWin32Impl::resize(u32 newWidth, u32 newHeight) {
        if (!m_Fullscreen){
            m_Width = newWidth;
            m_Height = newHeight;

            SR_LOG_WARNING_IF_FALSE(SetWindowPos(m_Hwnd, nullptr, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER), "SyriusWindowWin32Impl", "Failed to resize window");

        }
    }

    void SyriusWindowWin32Impl::requestFocus() {
        SR_LOG_WARNING_IF_FALSE(SetFocus(m_Hwnd), "SyriusWindowWin32Impl", "Failed to request focus");
    }

    void SyriusWindowWin32Impl::enableFullscreen() {
        if (!m_Fullscreen){
            // store current window info
            m_SavedWindowInfo.m_IsMaximized = IsZoomed(m_Hwnd) != 0;
            m_SavedWindowInfo.m_Style = GetWindowLongW(m_Hwnd, GWL_STYLE);
            m_SavedWindowInfo.m_ExStyle = GetWindowLongW(m_Hwnd, GWL_EXSTYLE);
            GetWindowRect(m_Hwnd, &m_SavedWindowInfo.m_Rect);

            // get monitor info
            MONITORINFO monitorInfo;
            monitorInfo.cbSize = sizeof(monitorInfo);
            HMONITOR monitor = MonitorFromWindow(m_Hwnd, MONITOR_DEFAULTTONEAREST);
            SR_LOG_WARNING_IF_FALSE(GetMonitorInfoW(monitor, &monitorInfo), "SyriusWindowWin32Impl", "Failed to get monitor info");

            // set window to fullscreen
            SR_LOG_WARNING_IF_FALSE(SetWindowLongW(m_Hwnd, GWL_STYLE, m_SavedWindowInfo.m_Style & ~(WS_CAPTION | WS_THICKFRAME)), "SyriusWindowWin32Impl", "Failed to set window style");
            SR_LOG_WARNING_IF_FALSE(SetWindowLongW(m_Hwnd, GWL_EXSTYLE, m_SavedWindowInfo.m_ExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE)), "SyriusWindowWin32Impl", "Failed to set window extended style");
            SR_LOG_WARNING_IF_FALSE(SetWindowPos(m_Hwnd, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED), "SyriusWindowWin32Impl", "Failed to set window position");


            m_Fullscreen = true;
        }
    }

    void SyriusWindowWin32Impl::disableFullscreen() {
        if (m_Fullscreen){
            // restore window
            SR_LOG_WARNING_IF_FALSE(SetWindowLongW(m_Hwnd, GWL_STYLE, m_SavedWindowInfo.m_Style), "SyriusWindowWin32Impl", "Failed to set window style");
            SR_LOG_WARNING_IF_FALSE(SetWindowLongW(m_Hwnd, GWL_EXSTYLE, m_SavedWindowInfo.m_ExStyle), "SyriusWindowWin32Impl", "Failed to set window extended style");
            SR_LOG_WARNING_IF_FALSE(SetWindowPos(m_Hwnd, nullptr, m_SavedWindowInfo.m_Rect.left, m_SavedWindowInfo.m_Rect.top, m_SavedWindowInfo.m_Rect.right - m_SavedWindowInfo.m_Rect.left, m_SavedWindowInfo.m_Rect.bottom - m_SavedWindowInfo.m_Rect.top, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED), "SyriusWindowWin32Impl", "Failed to set window position");
            if (m_SavedWindowInfo.m_IsMaximized){
                ShowWindow(m_Hwnd, SW_MAXIMIZE);
            }

            m_Fullscreen = false;
        }

    }

    void SyriusWindowWin32Impl::show() {
        SR_LOG_WARNING_IF_FALSE(ShowWindow(m_Hwnd, SW_SHOW), "SyriusWindowWin32Impl", "Failed to show window");
    }

    void SyriusWindowWin32Impl::hide() {
        SR_LOG_WARNING_IF_FALSE(ShowWindow(m_Hwnd, SW_HIDE), "SyriusWindowWin32Impl", "Failed to hide window");
    }

    void SyriusWindowWin32Impl::setTitle(const std::string &newTitle) {
        m_Title = newTitle;
        auto wTitle = stringToWideString(m_Title);

        SR_LOG_WARNING_IF_FALSE(SetWindowTextW(m_Hwnd, wTitle.c_str()), "SyriusWindowWin32Impl", "Failed to set window title");
    }

    void SyriusWindowWin32Impl::setIcon(const ImageFileDesc& desc, u32 icons) {
        auto img = createImage(desc);
        setIcon(img, icons);
    }

    void SyriusWindowWin32Impl::setIcon(const UP<Image>& image, u32 icons) {
        if (m_Icon){
            DestroyIcon(m_Icon);
            m_Icon = nullptr;
        }
        std::vector<UByte> bgra(image->getWidth() * image->getHeight() * image->getChannelCount());
        memcpy(&bgra[0], image->getData(), image->getWidth() * image->getHeight() * image->getChannelCount());
        for (u32 i = 0; i < image->getWidth() * image->getHeight() * image->getChannelCount(); i += image->getChannelCount()){
            std::swap(bgra[i], bgra[i + 2]);
        }
        m_Icon = CreateIcon(GetModuleHandleW(nullptr), image->getWidth(), image->getHeight(), 1, image->getChannelCount() * 8, nullptr, &bgra[0]);
        if (m_Icon){
            if (icons & SR_WINDOW_ICON_SMALL){
                SendMessageW(m_Hwnd, WM_SETICON, ICON_SMALL, (LPARAM)m_Icon);
            }
            if (icons & SR_WINDOW_ICON_BIG){
                SendMessageW(m_Hwnd, WM_SETICON, ICON_BIG, (LPARAM) m_Icon);
            }
        }
        else{
            m_Icon = nullptr;
        }

        SR_POSTCONDITION(m_Icon != nullptr, "Failed to set the window icon");
    }

    void SyriusWindowWin32Impl::pollEvents() {
        MSG msg;
        while (PeekMessageW(&msg, m_Hwnd, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    void SyriusWindowWin32Impl::setMousePosition(i32 mousePosX, i32 mousePosY) {
        auto relMousePosX = m_PosX + mousePosX;
        auto relMousePosY = m_PosY + mousePosY;

        SR_LOG_WARNING_IF_FALSE(SetCursorPos(relMousePosX, relMousePosY), "SyriusWindowWin32Impl", "Failed to set mouse position");
    }

    i32 SyriusWindowWin32Impl::getMousePositionX() {
        POINT mousePos;
        GetCursorPos(&mousePos);
        return mousePos.x;
    }

    i32 SyriusWindowWin32Impl::getMousePositionY() {
        POINT mousePos;
        GetCursorPos(&mousePos);
        return mousePos.y;
    }

    void SyriusWindowWin32Impl::hideMouse() {
        m_MouseVisible = false;
        while (ShowCursor(FALSE) >= 0);
    }

    void SyriusWindowWin32Impl::showMouse() {
        m_MouseVisible = true;
        while (ShowCursor(TRUE) < 0);
    }

    void SyriusWindowWin32Impl::grabMouse() {
        if (!m_MouseGrabbed){
            RECT windowDimmensions;
            GetClientRect(m_Hwnd, &windowDimmensions);
            MapWindowPoints(m_Hwnd, nullptr, reinterpret_cast<LPPOINT>(&windowDimmensions), 2);
            m_MouseGrabbed = ClipCursor(&windowDimmensions);
        }
    }

    void SyriusWindowWin32Impl::releaseMouse() {
        if (m_MouseGrabbed){
            SR_LOG_WARNING_IF_FALSE(ClipCursor(nullptr), "SyriusWindowWin32Impl", "Failed to release the mouse");
            m_MouseGrabbed = false;
        }

    }

    void SyriusWindowWin32Impl::centerWindow() {
        if (!m_Fullscreen){
            HDC screenDC = GetDC(nullptr);
            u32 width = GetDeviceCaps(screenDC, HORZRES);
            u32 height = GetDeviceCaps(screenDC, VERTRES);
            ReleaseDC(nullptr, screenDC);
            auto posX = static_cast<i32>((width - m_Width) / 2);
            auto posY = static_cast<i32>((height - m_Height) / 2);
            setPosition(posX, posY);
        }
    }

    void SyriusWindowWin32Impl::centerMouse() {
        u32 mouseX = m_Width / 2;
        u32 mouseY = m_Height / 2;
        setMousePosition(mouseX, mouseY);
    }

    std::string SyriusWindowWin32Impl::openFileDialog(const std::string &filter) {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = m_Hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter.c_str();
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameA(&ofn) == TRUE){
            return ofn.lpstrFile;
        }
        return "";
    }

    std::string SyriusWindowWin32Impl::saveFileDialog(const std::string &fileName, const std::string &filter) {
        OPENFILENAMEA ofn;
        char szFile[260];
        StringCbCopyA(szFile, fileName.size() + 1, fileName.c_str());
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = m_Hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter.c_str();
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetSaveFileNameA(&ofn) == TRUE){
            return ofn.lpstrFile;
        }
        return "";
    }

    ResourceView<Context> SyriusWindowWin32Impl::createContext(ContextDesc& desc) {
        switch (desc.api) {
            case SR_API_OPENGL: m_Context = createUP<WglContext>(m_Hwnd, desc);     break;
            case SR_API_D3D11:  m_Context = createUP<D3D11Context>(m_Hwnd, desc);   break;
            default:
                SR_LOG_WARNING("SyriusWindowWin32Impl", "cannot create context: unsupported API ({}), fallback to OpenGL!", desc.api);
                m_Context = createUP<WglContext>(m_Hwnd, desc);
                break;
        }
        return createResourceView(m_Context);
    }

    const HWND &SyriusWindowWin32Impl::getHwnd() const {
        return m_Hwnd;
    }

    LRESULT SyriusWindowWin32Impl::windowEventProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        if (msg == WM_CREATE){
            auto window = (LONG_PTR) reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams;
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, window);
        }
        // because this is a global function, we obtain our Syrius window instance stored in the HWND structure to
        // only handle events that belong to that specific window
        SyriusWindowWin32Impl* window = hwnd ? reinterpret_cast<SyriusWindowWin32Impl*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)) : nullptr;
        if (window != nullptr){
            // only handle events for ImGui for this window
            if (window->m_UseImGui){
                ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
                if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard){
                    window->m_CaptureMouseAndKeyboardEvents = false;
                }
                else{
                    window->m_CaptureMouseAndKeyboardEvents = true;
                }
            }
            window->handleEvent(msg, wparam, lparam);
        }
        // Prevents win32 from closing the window manually.
        if (msg == WM_CLOSE){
            return 0;
        }
        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    void SyriusWindowWin32Impl::handleEvent(UINT msg, WPARAM wparam, LPARAM lparam) {
        switch (msg){
            case WM_CREATE: {
                WindowOpenedEvent event;
                dispatchEvent(event);
                return;
            }
            case WM_DESTROY: {
                PostQuitMessage(0);
                return;
            }
            case WM_CLOSE: {
                WindowClosedEvent event;
                dispatchEvent(event);
                m_Open = false;
                return;
            }
            case WM_SIZE: {
                if (wparam != SIZE_MINIMIZED && !m_Resizing && !m_Moving && m_Open){
                    i32 newWidth = GET_X_LPARAM(lparam);
                    i32 newHeight = GET_Y_LPARAM(lparam);
                    m_Width = newWidth;
                    m_Height = newHeight;
                    WindowResizedEvent event(newWidth, newHeight);
                    dispatchEvent(event);
                    // If a context is present, get the new framebuffer size and update the context
                    if (m_Context) {
                        RECT clientSpace = {0, 0, 0, 0};
                        GetClientRect(m_Hwnd, &clientSpace);
                        m_Context->onResize(clientSpace.right, clientSpace.bottom);
                    }
                }
                break;
            }
            case WM_MOVE: {
                if (!m_Resizing && m_Open){
                    auto xPos = GET_X_LPARAM(lparam);
                    auto yPos = GET_Y_LPARAM(lparam);
                    WindowMovedEvent event(xPos, yPos);
                    m_PosX = xPos;
                    m_PosY = yPos;

                    dispatchEvent(event);
                }
                break;
            }
            case WM_SETFOCUS: {
                WindowGainedFocusEvent event;
                dispatchEvent(event);
                m_Focused = true;
                break;
            }
            case WM_KILLFOCUS: {
                if (m_Open){
                    WindowLostFocusEvent event;
                    dispatchEvent(event);
                    m_Focused = false;
                }
                break;
            }
            case WM_EXITSIZEMOVE: {
                m_Resizing = true;
                break;
            }
            case WM_ENTERSIZEMOVE: {
                m_Resizing = false;
                break;
            }
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                if (m_CaptureMouseAndKeyboardEvents){
                    auto key = convertVirtualKey(wparam, lparam);
                    KeyReleasedEvent event(key);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN: {
                if (m_CaptureMouseAndKeyboardEvents){
                    if (m_KeyRepeat || (HIWORD(lparam) & KF_REPEAT) == 0){
                        auto key = convertVirtualKey(wparam, lparam);
                        KeyPressedEvent event(key);
                        dispatchEvent(event);
                    }
                }
                break;
            }
            case WM_SYSCHAR:
            case WM_CHAR: {
                if (m_CaptureMouseAndKeyboardEvents){
                    if (m_KeyRepeat || (lparam & (1 << 30)) == 0){
                        auto ch = static_cast<unsigned int>(wparam);
                        KeyTypedEvent event(ch);
                        dispatchEvent(event);
                    }
                }
                break;
            }
            case WM_LBUTTONDOWN: {
                if (m_CaptureMouseAndKeyboardEvents){
                    MouseButtonPressedEvent event(SR_MOUSE_BUTTON_LEFT);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_MBUTTONDOWN: {
                if (m_CaptureMouseAndKeyboardEvents){
                    MouseButtonPressedEvent event(SR_MOUSE_BUTTON_MIDDLE);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_RBUTTONDOWN: {
                if (m_CaptureMouseAndKeyboardEvents){
                    MouseButtonPressedEvent event(SR_MOUSE_BUTTON_RIGHT);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_LBUTTONUP: {
                if (m_CaptureMouseAndKeyboardEvents){
                    MouseButtonReleasedEvent event(SR_MOUSE_BUTTON_LEFT);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_MBUTTONUP: {
                if (m_CaptureMouseAndKeyboardEvents){
                    MouseButtonReleasedEvent event(SR_MOUSE_BUTTON_MIDDLE);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_RBUTTONUP: {
                if (m_CaptureMouseAndKeyboardEvents){
                    MouseButtonReleasedEvent event(SR_MOUSE_BUTTON_RIGHT);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_XBUTTONDOWN: {
                if (m_CaptureMouseAndKeyboardEvents){
                    SR_MOUSE_BUTTON button = HIWORD(lparam) == XBUTTON1 ? SR_MOUSE_BUTTON_X1 : SR_MOUSE_BUTTON_X2;
                    MouseButtonPressedEvent event(button);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_XBUTTONUP: {
                if (m_CaptureMouseAndKeyboardEvents){
                    SR_MOUSE_BUTTON button = HIWORD(lparam) == XBUTTON1 ? SR_MOUSE_BUTTON_X1 : SR_MOUSE_BUTTON_X2;
                    MouseButtonReleasedEvent event(button);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_MOUSELEAVE: {
                if (m_MouseInside && m_CaptureMouseAndKeyboardEvents){
                    m_MouseInside = false;
                    MouseLeftEvent event;
                    dispatchEvent(event);
                }
                break;
            }
            case WM_MOUSEMOVE: {
                if (m_CaptureMouseAndKeyboardEvents){
                    auto mouseX = GET_X_LPARAM(lparam);
                    auto mouseY = GET_Y_LPARAM(lparam);
                    RECT window;
                    GetClientRect(m_Hwnd, &window);
                    if ((wparam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0){
                        if (GetCapture() == m_Hwnd){
                            ReleaseCapture();
                        }
                    }
                    else if (GetCapture() != m_Hwnd){
                        SetCapture(m_Hwnd);
                    }
                    if ((mouseX < window.left) || (mouseX > window.right) || (mouseY < window.top) || (mouseY > window.bottom)){
                        if (m_MouseInside){
                            m_MouseInside = false;
                            mouseTracker(false);
                            MouseLeftEvent event;
                            dispatchEvent(event);
                        }
                    }
                    else{
                        if (!m_MouseInside){
                            m_MouseInside = true;
                            mouseTracker(true);
                            MouseEnteredEvent event;
                            dispatchEvent(event);
                        }
                    }
                    if (m_MouseInside){
                        MouseMovedEvent event(mouseX, mouseY);
                        dispatchEvent(event);
                    }
                }
                break;
            }
            case WM_MOUSEWHEEL: {
                if (m_MouseInside && m_CaptureMouseAndKeyboardEvents){
                    i32 xOffset = LOWORD(lparam);
                    i32 yOffset = HIWORD(lparam);
                    MouseScrolledEvent event(xOffset, yOffset);
                    dispatchEvent(event);
                }
                break;
            }
            // raw input from devices
            case WM_INPUT: {
                u32 size;
                if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1){
                    break;
                }
                std::vector<u8> buffer(size);
                if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, buffer.data(), &size, sizeof(RAWINPUTHEADER)) != size){
                    break;
                }
                auto raw = reinterpret_cast<RAWINPUT*>(buffer.data());
                switch (raw->header.dwType) {
                    case RIM_TYPEMOUSE: {
                        if (m_MouseInside and m_CaptureMouseAndKeyboardEvents){
                            if (raw->data.mouse.lLastX != 0 and raw->data.mouse.lLastY != 0 ){
                                RawMouseMovedEvent event(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                                dispatchEvent(event);
                            }
                        }
                        break;
                    }
                    case RIM_TYPEKEYBOARD: {
                        auto key = convertVirtualKey(raw->data.keyboard.VKey, raw->data.keyboard.Flags);
                        if (raw->data.keyboard.Flags & RI_KEY_BREAK){
                            RawKeyPressedEvent event(key);
                            dispatchEvent(event);
                        }
                        else{
                            RawKeyReleasedEvent event(key);
                            dispatchEvent(event);
                        }
                        break;
                    }
                    default: break;
                }
                break;
            }
            case WM_DROPFILES: {
                auto hDrop = reinterpret_cast<HDROP>(wparam);
                u32 count = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
                for (u32 i = 0; i < count; ++i){
                    u32 length = DragQueryFile(hDrop, i, nullptr, 0);
                    std::string file(length, '\0');
                    DragQueryFile(hDrop, i, file.data(), length + 1);
                    WindowFileDroppedEvent event(file);
                    dispatchEvent(event);
                }
                DragFinish(hDrop);
                break;
            }
            default: break;
        }
    }

    void SyriusWindowWin32Impl::mouseTracker(bool enableTracking) const {
        TRACKMOUSEEVENT mouseEvent;
        mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        if (enableTracking){
            mouseEvent.dwFlags = TME_LEAVE;
        }
        else{
            mouseEvent.dwFlags = TME_CANCEL;
        }
        mouseEvent.hwndTrack = m_Hwnd;
        mouseEvent.dwHoverTime = HOVER_DEFAULT;
        TrackMouseEvent(&mouseEvent);
    }

    void SyriusWindowWin32Impl::registerWindowClass() {
        WNDCLASSEXW wndClass;
        wndClass.cbSize = sizeof(WNDCLASSEXW);
        wndClass.style = 0;
        wndClass.lpfnWndProc = &SyriusWindowWin32Impl::windowEventProc;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = GetModuleHandleW(nullptr);
        wndClass.hIcon = nullptr;
        wndClass.hCursor = nullptr;
        wndClass.hbrBackground = nullptr;
        wndClass.lpszMenuName = L"SYRIUS_CORE";
        wndClass.lpszClassName = s_SyriusWindowClass;
        wndClass.hIconSm = nullptr;

        RegisterClassExW(&wndClass);

        SR_CORE_HRESULT(GetLastError());
    }

    void SyriusWindowWin32Impl::unregisterWindowClass() {
        UnregisterClassW(s_SyriusWindowClass, GetModuleHandleW(nullptr));
    }

    void SyriusWindowWin32Impl::setProcessDPIAwareness() {
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
                    SR_LOG_WARNING("SyriusWindowWin32Impl", "Failed to set process DPI awareness using shCore.dll libary, falling back on user32.dll");
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
                SR_LOG_WARNING_IF_FALSE(setProcessDPIAwareFunc(), "SyriusWindowWin32Impl", "Failed to set process DPI awareness");
            }
            FreeLibrary(user32Dll);

        }
    }

}

#endif