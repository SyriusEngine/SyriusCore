#include "SyriusWindowX11Impl.hpp"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    SyriusWindowX11Impl::SyriusWindowX11Impl(const WindowDesc &desc, Display* display)
    : SyriusWindow(desc),
    m_Display(display),
      m_SkipNextWindowCloseEvent(false){
        m_Window = XCreateSimpleWindow(m_Display, DefaultRootWindow(m_Display),
                                       m_PosX, m_PosY,
                                       m_Width, m_Height,
                                       1, 0, 0);
        XSelectInput(m_Display, m_Window, FocusChangeMask      | ButtonPressMask     |
                                                            ButtonReleaseMask    | ButtonMotionMask    |
                                                            PointerMotionMask    | KeyPressMask        |
                                                            KeyReleaseMask       | StructureNotifyMask |
                                                            EnterWindowMask      | LeaveWindowMask     |
                                                            VisibilityChangeMask | PropertyChangeMask);
        XMapWindow(m_Display, m_Window);
        XStoreName(m_Display, m_Window, m_Title.c_str());
        XFlush(m_Display);
        m_Open = true;
    }

    SyriusWindowX11Impl::~SyriusWindowX11Impl() {
        XDestroyWindow(m_Display, m_Window);
    }

    void SyriusWindowX11Impl::close() {
        m_Open = false;
    }

    void SyriusWindowX11Impl::setPosition(int32 posX, int32 posY) {
        XMoveWindow(m_Display, m_Window, posX, posY);
        m_PosX = posX;
        m_PosY = posY;
    }

    void SyriusWindowX11Impl::resize(uint32 newWidth, uint32 newHeight) {
        XResizeWindow(m_Display, m_Window, newWidth, newHeight);
        m_Width = newWidth;
        m_Height = newHeight;
    }

    void SyriusWindowX11Impl::requestFocus() {
        XRaiseWindow(m_Display, m_Window);
    }

    void SyriusWindowX11Impl::enableFullscreen() {
        Atom wm_state = XInternAtom(m_Display, "_NET_WM_STATE", False);
        Atom fullscreen = XInternAtom(m_Display, "_NET_WM_STATE_FULLSCREEN", False);
        XEvent xev;
        memset(&xev, 0, sizeof(xev));
        xev.type = ClientMessage;
        xev.xclient.window = m_Window;
        xev.xclient.message_type = wm_state;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = 1;
        xev.xclient.data.l[1] = fullscreen;
        xev.xclient.data.l[2] = 0;
        XSendEvent(m_Display, DefaultRootWindow(m_Display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
    }

    void SyriusWindowX11Impl::disableFullscreen() {
        Atom wm_state = XInternAtom(m_Display, "_NET_WM_STATE", False);
        Atom fullscreen = XInternAtom(m_Display, "_NET_WM_STATE_FULLSCREEN", False);
        XEvent xev;
        memset(&xev, 0, sizeof(xev));
        xev.type = ClientMessage;
        xev.xclient.window = m_Window;
        xev.xclient.message_type = wm_state;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = 0;
        xev.xclient.data.l[1] = fullscreen;
        xev.xclient.data.l[2] = 0;
        XSendEvent(m_Display, DefaultRootWindow(m_Display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
    }

    void SyriusWindowX11Impl::show() {
        XMapWindow(m_Display, m_Window);
    }

    void SyriusWindowX11Impl::hide() {
        XUnmapWindow(m_Display, m_Window);
    }

    void SyriusWindowX11Impl::setTitle(const std::string &newTitle) {
        XStoreName(m_Display, m_Window, newTitle.c_str());
        m_Title = newTitle;
    }

    void SyriusWindowX11Impl::setIcon(const std::string &imagePath, bool flipVertically, uint32 icons) {

    }

    void SyriusWindowX11Impl::setIcon(Image *image, uint32 icons) {

    }

    void SyriusWindowX11Impl::pollEvents() {
        XEvent event;
        while (XPending(m_Display)) {
            XNextEvent(m_Display, &event);
            switch (event.type) {
                case DestroyNotify: {
                    if (event.xdestroywindow.window == m_Window) {
                        /**
                         * Due to the nature of X11, we recreate the window each time a new context is created.
                         * During this process, a windowClosedEvent is sent but we do not want to send this event
                         * to the user as it is not a real window close event for the application concern.
                         */
                        if (!m_SkipNextWindowCloseEvent){
                            dispatchEvent(WindowClosedEvent());
                        }
                        else{
                            m_SkipNextWindowCloseEvent = false;
                        }
                    }
                    break;
                }
                case FocusIn: {
                    if (!m_Resizing){
                        WindowGainedFocusEvent srEvent;
                        dispatchEvent(srEvent);
                    }
                    break;
                }
                case FocusOut: {
                    if (!m_Resizing){
                        WindowLostFocusEvent srEvent;
                        dispatchEvent(srEvent);
                    }
                    break;
                }
                case ClientMessage: {
                    if (event.xclient.data.l[0] == XInternAtom(m_Display, "WM_DELETE_WINDOW", False)) {
                        dispatchEvent(WindowClosedEvent());
                    }
                    break;
                }
                case ConfigureNotify: {
                    if (event.xconfigure.width != m_Width || event.xconfigure.height != m_Height) {
                        m_Resizing = true;
                        WindowResizedEvent srEvent(event.xconfigure.width, event.xconfigure.height);
                        dispatchEvent(srEvent);
                        m_Resizing = false;
                    }
                    else if (event.xconfigure.x != m_PosX || event.xconfigure.y != m_PosY) {
                        WindowMovedEvent srEvent(event.xconfigure.x, event.xconfigure.y);
                        dispatchEvent(srEvent);
                    }
                    break;
                }
                case ButtonPress: {
                    if (event.xbutton.button == Button1) {
                        dispatchEvent(MouseButtonPressedEvent(SR_MOUSE_BUTTON_LEFT));
                    } else if (event.xbutton.button == Button2) {
                        dispatchEvent(MouseButtonPressedEvent(SR_MOUSE_BUTTON_MIDDLE));
                    } else if (event.xbutton.button == Button3) {
                        dispatchEvent(MouseButtonPressedEvent(SR_MOUSE_BUTTON_RIGHT));
                    }
                    break;
                }
                case ButtonRelease: {
                    if (event.xbutton.button == Button1){
                        dispatchEvent(MouseButtonReleasedEvent(SR_MOUSE_BUTTON_LEFT));
                    }
                    else if (event.xbutton.button == Button2){
                        dispatchEvent(MouseButtonReleasedEvent(SR_MOUSE_BUTTON_MIDDLE));
                    }
                    else if (event.xbutton.button == Button3){
                        dispatchEvent(MouseButtonReleasedEvent(SR_MOUSE_BUTTON_RIGHT));
                    }
                    break;
                }
                case KeymapNotify: {
                    XRefreshKeyboardMapping(&event.xmapping);
                }
                case KeyPress :{
                    SR_KEYBOARD_KEY key = SR_KEY_NONE;
                    for (uint32 i = 0; i < 4 ; i++){
                        key = convertVirtualKey(XLookupKeysym(&event.xkey, i));
                        if (key != SR_KEY_NONE){
                            dispatchEvent(KeyPressedEvent(key));
                            break;
                        }
                    }

                    char str[25] = {0};
                    if (XLookupString(&event.xkey, str, 25, nullptr, nullptr)) {
                        KeyTypedEvent srEvent(str[0]);
                        dispatchEvent(srEvent);
                    }
                    break;
                }
                case KeyRelease: {
                    SR_KEYBOARD_KEY key = SR_KEY_NONE;
                    for (uint32 i = 0; i < 4 ; i++){
                        key = convertVirtualKey(XLookupKeysym(&event.xkey, i));
                        if (key != SR_KEY_NONE){
                            dispatchEvent(KeyReleasedEvent(key));
                            break;
                        }
                    }
                    break;
                }
                case MotionNotify: {
                    MouseMovedEvent srEvent(event.xmotion.x, event.xmotion.y);
                    dispatchEvent(srEvent);
                    break;
                }
                case EnterNotify: dispatchEvent(MouseEnteredEvent()); break;
                case LeaveNotify: dispatchEvent(MouseLeftEvent()); break;
            }
        }
    }

    void SyriusWindowX11Impl::setMousePosition(int32 mousePosX, int32 mousePosY) {
        XWarpPointer(m_Display, None, m_Window, 0, 0, 0, 0, mousePosX, mousePosY);
    }

    int32 SyriusWindowX11Impl::getMousePositionX() {
        int32 rootX, rootY, winX, winY;
        uint32 mask;
        Window root, child;
        XQueryPointer(m_Display, m_Window, &root, &child, &rootX, &rootY, &winX, &winY, &mask);
        return winX;
    }

    int32 SyriusWindowX11Impl::getMousePositionY() {
        int32 rootX, rootY, winX, winY;
        uint32 mask;
        Window root, child;
        XQueryPointer(m_Display, m_Window, &root, &child, &rootX, &rootY, &winX, &winY, &mask);
        return winY;
    }

    void SyriusWindowX11Impl::hideMouse() {
    }

    void SyriusWindowX11Impl::showMouse() {

    }

    void SyriusWindowX11Impl::grabMouse() {

    }

    void SyriusWindowX11Impl::releaseMouse() {

    }

    void SyriusWindowX11Impl::centerWindow() {
        int32 screenX = XWidthOfScreen(XDefaultScreenOfDisplay(m_Display));
        int32 screenY = XHeightOfScreen(XDefaultScreenOfDisplay(m_Display));
        int32 posX = (screenX - m_Width) / 2;
        int32 posY = (screenY - m_Height) / 2;
        XMoveWindow(m_Display, m_Window, posX, posY);
    }

    void SyriusWindowX11Impl::centerMouse() {
        int32 screenX = XWidthOfScreen(XDefaultScreenOfDisplay(m_Display));
        int32 screenY = XHeightOfScreen(XDefaultScreenOfDisplay(m_Display));
        int32 posX = (screenX - m_Width) / 2;
        int32 posY = (screenY - m_Height) / 2;
        XWarpPointer(m_Display, None, m_Window, 0, 0, 0, 0, posX, posY);

    }

    std::string SyriusWindowX11Impl::openFileDialog(const std::string &filter) {
        return std::string();
    }

    std::string SyriusWindowX11Impl::saveFileDialog(const std::string &fileName, const std::string &filter) {
        return std::string();
    }

    Context *SyriusWindowX11Impl::createContext(const ContextDesc &desc) {
        switch (desc.m_API) {
            case SR_API_OPENGL:
                m_SkipNextWindowCloseEvent = true;
                m_Context = new GlxContext(m_Display, m_Window, desc);
                XMapWindow(m_Display, m_Window);
                XStoreName(m_Display, m_Window, m_Title.c_str());
                XFlush(m_Display);
                return m_Context;
            default:
                SR_CORE_WARNING("cannot create context: %i", desc.m_API);
                return nullptr;
        }
    }
}

#endif
