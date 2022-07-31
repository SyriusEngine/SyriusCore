#include "SyriusWindowWin32Impl.hpp"
#include "../Core/CoreCommand.hpp"
#include "Context/WglContext.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    uint32 SyriusWindowWin32Impl::m_WindowCount = 0;

    SyriusWindowWin32Impl::SyriusWindowWin32Impl(const WindowDesc &desc)
    : SyriusWindow(desc),
      m_Hwnd(nullptr),
      m_Callback(0),
      m_Icon(nullptr){
        if (!m_WindowCount){
            setProcessDpiAware();
            registerClass();
        }

        DWORD winStyles = decodeWindowStyle(desc.m_Style);
        DWORD exWinStyles = decodeExtendedWindowStyle(desc.m_Style);

        auto wTitle = stringToWideString(desc.m_Title);
        m_Hwnd = CreateWindowExW(exWinStyles,
                                 s_SyriusWindowClass,
                                 wTitle.c_str(),
                                 winStyles,
                                 desc.m_PosX, desc.m_PosY,
                                 desc.m_Width, desc.m_Height,
                                 nullptr, nullptr,
                                 GetModuleHandleW(nullptr),
                                 this);
        if (m_Hwnd){
            m_WindowCount++;

            ShowWindow(m_Hwnd, SW_SHOW);
            m_Open = true;
            m_Focused = true;

            // store a pointer to our SyriusWindow on the Win32 API side
            SetWindowLongPtrW(m_Hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            m_Callback = SetWindowLongPtrW(m_Hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&SyriusWindowWin32Impl::windowEventProc));

        }
        else{
            SR_CORE_WARNING("Failed to create window");
        }
    }

    SyriusWindowWin32Impl::~SyriusWindowWin32Impl() {
        if (m_Hwnd){
            DestroyWindow(m_Hwnd);
            m_WindowCount--;
            if (m_WindowCount == 0){
                unregisterClass();
            }
        }
    }

    void SyriusWindowWin32Impl::close() {
        m_Open = false;
        ShowWindow(m_Hwnd, SW_HIDE);
    }

    void SyriusWindowWin32Impl::setPosition(int32 posX, int32 posY) {
        m_PosX = posX;
        m_PosY = posY;

        SR_CORE_CHECK_CALL(SetWindowPos(m_Hwnd, nullptr, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER), "Failed to set window position");
    }

    void SyriusWindowWin32Impl::resize(uint32 newWidth, uint32 newHeight) {
        m_Width = newWidth;
        m_Height = newHeight;

        SR_CORE_CHECK_CALL(SetWindowPos(m_Hwnd, nullptr, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER), "Failed to resize window");
    }

    void SyriusWindowWin32Impl::requestFocus() {
        SR_CORE_CHECK_CALL(SetFocus(m_Hwnd), "Failed to request focus");
    }

    void SyriusWindowWin32Impl::enableFullscreen() {

    }

    void SyriusWindowWin32Impl::disableFullscreen() {

    }

    void SyriusWindowWin32Impl::show() {
        SR_CORE_CHECK_CALL(ShowWindow(m_Hwnd, SW_SHOW), "Failed to show window");
    }

    void SyriusWindowWin32Impl::hide() {
        SR_CORE_CHECK_CALL(ShowWindow(m_Hwnd, SW_HIDE), "Failed to hide window");
    }

    void SyriusWindowWin32Impl::setTitle(const std::string &newTitle) {
        m_Title = newTitle;
        auto wTitle = stringToWideString(m_Title);

        SR_CORE_CHECK_CALL(SetWindowTextW(m_Hwnd, wTitle.c_str()), "Failed to set window title");
    }

    void SyriusWindowWin32Impl::setIcon(const std::string &imagePath, bool flipVertically, uint32 icons) {
        auto img = new Image(imagePath, flipVertically);
        setIcon(img, icons);
        delete img;
    }

    void SyriusWindowWin32Impl::setIcon(Image *image, uint32 icons) {
        if (m_Icon){
            DestroyIcon(m_Icon);
            m_Icon = nullptr;
        }
        m_Icon = CreateIcon(GetModuleHandleW(nullptr), image->getWidth(), image->getHeight(), 1, image->getChannelCount() * 8, nullptr, &image->getPixelsBGRA()[0]);
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

        SR_CORE_POSTCONDITION(m_Icon != nullptr, "Failed to set the window icon");
    }

    void SyriusWindowWin32Impl::pollEvents() {
        MSG msg;
        while (PeekMessageW(&msg, m_Hwnd, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    void SyriusWindowWin32Impl::setMousePosition(int32 mousePosX, int32 mousePosY) {
        auto relMousePosX = m_PosX + mousePosX;
        auto relMousePosY = m_PosY + mousePosY;

        SR_CORE_CHECK_CALL(SetCursorPos(relMousePosX, relMousePosY), "Failed to set mouse position");
    }

    int32 SyriusWindowWin32Impl::getMousePositionX() {
        POINT mousePos;
        GetCursorPos(&mousePos);
        return mousePos.x;
    }

    int32 SyriusWindowWin32Impl::getMousePositionY() {
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

        SR_CORE_POSTCONDITION(m_MouseGrabbed == true, "Failed to grab the mouse")
    }

    void SyriusWindowWin32Impl::releaseMouse() {
        if (m_MouseGrabbed){
            m_MouseGrabbed = ClipCursor(nullptr);
        }

        SR_CORE_POSTCONDITION(m_MouseGrabbed == false, "Failed to release mouse")
    }

    void SyriusWindowWin32Impl::centerWindow() {
        auto posX = static_cast<int32>((CoreCommand::getPrimaryScreenWidth() - m_Width) / 2);
        auto posY = static_cast<int32>((CoreCommand::getPrimaryScreenHeight() - m_Height) / 2);
        setPosition(posX, posY);
    }

    void SyriusWindowWin32Impl::centerMouse() {
        uint32 mouseX = m_Width / 2;
        uint32 mouseY = m_Height / 2;
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
        else{
            return "";
        }
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
        else{
            return "";
        }
    }

    Context* SyriusWindowWin32Impl::requestContext(SR_SUPPORTED_API api) {
        switch (api) {
            case SR_API_OPENGL:
                m_Context = new WglContext(m_Hwnd);
                return m_Context;
            default:
                SR_CORE_WARNING("cannot create context: unsupported API")
                return nullptr;
        }

    }

    const HWND &SyriusWindowWin32Impl::getHwnd() const {
        return m_Hwnd;
    }

    LRESULT SyriusWindowWin32Impl::windowEventProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        if (m_ImGuiInstances){
            ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
        }
        if (msg == WM_CREATE){
            auto window = (LONG_PTR) reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams;
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, window);
        }
        // because this is a global function, we obtain our Syrius window instance stored in the HWND structure to
        // only handle events that belong to that specific window
        SyriusWindowWin32Impl* window = hwnd ? reinterpret_cast<SyriusWindowWin32Impl*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)) : nullptr;
        if (window != nullptr){
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
                    int32_t newWidth = GET_X_LPARAM(lparam);
                    int32_t newHeight = GET_Y_LPARAM(lparam);
                    m_Width = newWidth;
                    m_Height = newHeight;
                    WindowResizedEvent event(newWidth, newHeight);
                    dispatchEvent(event);
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
                auto key = convertVirtualKey(wparam, lparam);
                KeyReleasedEvent event(key);
                dispatchEvent(event);
                break;
            }
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN: {
                if (m_KeyRepeat || (HIWORD(lparam) & KF_REPEAT) == 0){
                    auto key = convertVirtualKey(wparam, lparam);
                    KeyPressedEvent event(key);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_SYSCHAR:
            case WM_CHAR: {
                if (m_KeyRepeat || (lparam & (1 << 30)) == 0){
                    auto ch = static_cast<unsigned int>(wparam);
                    KeyTypedEvent event(ch);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_LBUTTONDOWN: {
                MouseButtonPressedEvent event(SR_MOUSE_BUTTON_LEFT);
                dispatchEvent(event);
                break;
            }
            case WM_MBUTTONDOWN: {
                MouseButtonPressedEvent event(SR_MOUSE_BUTTON_MIDDLE);
                dispatchEvent(event);
                break;
            }
            case WM_RBUTTONDOWN: {
                MouseButtonPressedEvent event(SR_MOUSE_BUTTON_RIGHT);
                dispatchEvent(event);
                break;
            }
            case WM_LBUTTONUP: {
                MouseButtonReleasedEvent event(SR_MOUSE_BUTTON_LEFT);
                dispatchEvent(event);
                break;
            }
            case WM_MBUTTONUP: {
                MouseButtonReleasedEvent event(SR_MOUSE_BUTTON_MIDDLE);
                dispatchEvent(event);
                break;
            }
            case WM_RBUTTONUP: {
                MouseButtonReleasedEvent event(SR_MOUSE_BUTTON_RIGHT);
                dispatchEvent(event);
                break;
            }
            case WM_XBUTTONDOWN: {
                SR_MOUSE_BUTTON button = HIWORD(lparam) == XBUTTON1 ? SR_MOUSE_BUTTON_X1 : SR_MOUSE_BUTTON_X2;
                MouseButtonPressedEvent event(button);
                dispatchEvent(event);
                break;
            }
            case WM_XBUTTONUP: {
                SR_MOUSE_BUTTON button = HIWORD(lparam) == XBUTTON1 ? SR_MOUSE_BUTTON_X1 : SR_MOUSE_BUTTON_X2;
                MouseButtonReleasedEvent event(button);
                dispatchEvent(event);
                break;
            }
            case WM_MOUSELEAVE: {
                if (m_MouseInside){
                    m_MouseInside = false;
                    MouseLeftEvent event;
                    dispatchEvent(event);
                }
                break;
            }
            case WM_MOUSEMOVE: {
                auto mouseX = static_cast<int32_t>(GET_X_LPARAM(lparam));
                auto mouseY = static_cast<int32_t>(GET_Y_LPARAM(lparam));
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
                break;
            }
            case WM_MOUSEWHEEL: {
                if (m_MouseInside){
                    int32_t xOffset = LOWORD(lparam);
                    int32_t yOffset = HIWORD(lparam);
                    MouseScrolledEvent event(xOffset, yOffset);
                    dispatchEvent(event);
                }
                break;
            }
            case WM_DROPFILES: {
                /// maybe you need to call DragAcceptFiles with the fAccept param = true but how to get the filename?
                SR_CORE_MESSAGE("A file has been dropped in the window -> no idea how to get the file name xD")
            }
            default: break;
        }
    }

    void SyriusWindowWin32Impl::mouseTracker(bool enableTracking) {
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

    void SyriusWindowWin32Impl::registerClass() {
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

    void SyriusWindowWin32Impl::unregisterClass() {
        UnregisterClassW(s_SyriusWindowClass, GetModuleHandleW(nullptr));
    }

}

#endif