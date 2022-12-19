#include "SyriusWindowX11Impl.hpp"

#if defined(SR_CORE_PLATFORM_LINUX)

namespace Syrius{

    SyriusWindowX11Impl::SyriusWindowX11Impl(const WindowDesc &desc, Display* display)
    : SyriusWindow(desc),
    m_Display(display){
        // by default, init GLX
        auto glxDesc = new GlPlatformDescX11();
        CoreCommand::initPlatformGlad(glxDesc);
        delete glxDesc;

        XVisualInfo *visualInfo = selectBestVisual();
        XSetWindowAttributes attributes;
        attributes.colormap = XCreateColormap(m_Display, DefaultRootWindow(m_Display), visualInfo->visual, AllocNone);
        attributes.event_mask = FocusChangeMask      | ButtonPressMask     |
                                ButtonReleaseMask    | ButtonMotionMask    |
                                PointerMotionMask    | KeyPressMask        |
                                KeyReleaseMask       | StructureNotifyMask |
                                EnterWindowMask      | LeaveWindowMask     |
                                VisibilityChangeMask | PropertyChangeMask;
        m_Window = XCreateWindow(m_Display, DefaultRootWindow(m_Display),
                                 desc.m_PosX, desc.m_PosY,
                                 desc.m_Width, desc.m_Height,
                                 0, visualInfo->depth, InputOutput, visualInfo->visual, CWColormap | CWEventMask, &attributes);
        if (m_Window){
            m_Open = true;
            setWindowProtocols();
            setWindowStyles(desc.m_Style);
            XMapWindow(m_Display, m_Window);
            XStoreName(m_Display, m_Window, desc.m_Title.c_str());

            // set window styles
        }
        else{
            SR_CORE_WARNING("Failed to create window with X11 backend");
        }
        XFree(visualInfo);
    }

    SyriusWindowX11Impl::~SyriusWindowX11Impl() {
        CoreCommand::terminatePlatformGlad();
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
                    dispatchEvent(WindowClosedEvent());
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
                m_Context = new GlxContext(m_Display, m_BestFBConfig, m_Window, desc);
                return m_Context;
            default:
                SR_CORE_WARNING("cannot create context: %i", desc.m_API);
                return nullptr;
        }
    }

    XVisualInfo *SyriusWindowX11Impl::selectBestVisual() {
        int32 glxMajorVersion = 0;
        int32 glxMinorVersion = 0;
        if (!glXQueryVersion(m_Display, &glxMajorVersion, &glxMinorVersion)) {
            SR_CORE_EXCEPTION("Failed to query GLX version");
            return nullptr;
        }
        else{
            SR_CORE_MESSAGE("GLX version: %i.%i", glxMajorVersion, glxMinorVersion);
        }

        int32 visualAttribs[] = {
                GLX_X_RENDERABLE    , True,
                GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
                GLX_RENDER_TYPE     , GLX_RGBA_BIT,
                GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
                GLX_DOUBLEBUFFER    , True,
                None
        };

        // pick the best FrameBufferConfig
        int32 fbCount = 0;
        GLXFBConfig *fbc = glXChooseFBConfig(m_Display, DefaultScreen(m_Display), visualAttribs, &fbCount);
        int32 bestFbc = -1, worstFbc = -1, bestNumSamp = -1, worstNumSamp = 999;
        for (int32 i = 0; i < fbCount; i++) {
            XVisualInfo *vi = glXGetVisualFromFBConfig(m_Display, fbc[i]);
            if (vi) {
                int32 sampBuf, samples;
                glXGetFBConfigAttrib(m_Display, fbc[i], GLX_SAMPLE_BUFFERS, &sampBuf);
                glXGetFBConfigAttrib(m_Display, fbc[i], GLX_SAMPLES, &samples);
                if (bestFbc < 0 || sampBuf && samples > bestNumSamp)
                    bestFbc = i, bestNumSamp = samples;
                if (worstFbc < 0 || !sampBuf || samples < worstNumSamp)
                    worstFbc = i, worstNumSamp = samples;
            }
            XFree(vi);
        }
        SR_CORE_ASSERT(bestFbc >= 0, "Failed to find a suitable FBConfig");
        // get the best visual
        m_BestFBConfig = fbc[bestFbc];
        XFree(fbc);
        return glXGetVisualFromFBConfig(m_Display, m_BestFBConfig);
    }

    void SyriusWindowX11Impl::setWindowProtocols() {
        Atom wmProtocols = XInternAtom(m_Display, "WM_PROTOCOLS", False);
        if (wmProtocols == None) {
            SR_CORE_WARNING("Failed to get WM_PROTOCOLS atom");
            return;
        }
        Atom wmDelete = XInternAtom(m_Display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_Display, m_Window, &wmDelete, 1);

    }

    void SyriusWindowX11Impl::setWindowStyles(SR_WINDOW_STYLE style) {
        Atom wmHints = XInternAtom(m_Display, "_MOTIF_WM_HINTS", False);
        if (wmHints == None) {
            SR_CORE_WARNING("Failed to get _MOTIF_WM_HINTS atom");
            return;
        }
        static const unsigned long MWM_HINTS_FUNCTIONS   = 1 << 0;
        static const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;

        //static const unsigned long MWM_DECOR_ALL         = 1 << 0;
        static const unsigned long MWM_DECOR_BORDER      = 1 << 1;
        static const unsigned long MWM_DECOR_RESIZEH     = 1 << 2;
        static const unsigned long MWM_DECOR_TITLE       = 1 << 3;
        static const unsigned long MWM_DECOR_MENU        = 1 << 4;
        static const unsigned long MWM_DECOR_MINIMIZE    = 1 << 5;
        static const unsigned long MWM_DECOR_MAXIMIZE    = 1 << 6;

        //static const unsigned long MWM_FUNC_ALL          = 1 << 0;
        static const unsigned long MWM_FUNC_RESIZE       = 1 << 1;
        static const unsigned long MWM_FUNC_MOVE         = 1 << 2;
        static const unsigned long MWM_FUNC_MINIMIZE     = 1 << 3;
        static const unsigned long MWM_FUNC_MAXIMIZE     = 1 << 4;
        static const unsigned long MWM_FUNC_CLOSE        = 1 << 5;

        struct MwmHints {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long inputMode;
            unsigned long status;
        };
        MwmHints hints = {0};
        hints.flags =  MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
        hints.decorations = 0;
        hints.functions = 0;

        if (style & SR_WINDOW_STYLE_TITLEBAR) {
            hints.decorations |= MWM_DECOR_TITLE | MWM_DECOR_MENU;
            hints.functions |= MWM_FUNC_MOVE;
        }
        if (style & SR_WINDOW_STYLE_RESIZE) {
            hints.decorations |= MWM_DECOR_RESIZEH | MWM_DECOR_MAXIMIZE | MWM_DECOR_MINIMIZE;
            hints.functions |= MWM_FUNC_RESIZE;
        }
        if (style & SR_WINDOW_STYLE_CLOSE) {
            hints.functions |= MWM_FUNC_CLOSE;
        }




        XChangeProperty(m_Display, m_Window, wmHints, wmHints, 32, PropModeReplace, (unsigned char *) &hints, 5);

    }
}

#endif
