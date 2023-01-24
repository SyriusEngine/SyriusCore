#include "CoreCommand.hpp"
#include "PlatformAPIWin32Impl.hpp"
#include "PlatformAPIX11Impl.hpp"

namespace Syrius{

    CoreCommand* CoreCommand::m_Instance = nullptr;

    CoreCommand::CoreCommand():
    m_StartupTime(0),
    m_GladInstances(0) {
        if (m_Instance){
            SR_CORE_WARNING("SyriusCore is already initialized");
            return;
        }
        else{
            m_Instance = this;
            m_StartupTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();

            SR_CORE_MESSAGE("Initializing Syrius")

#if defined(SR_CORE_PLATFORM_WIN64)
            m_PlatformAPI = Resource<PlatformAPI>(new PlatformAPIWin32Impl());
#elif defined(SR_CORE_PLATFORM_LINUX)
            m_PlatformAPI = Resource<PlatformAPI>(new PlatformAPIX11Impl());
#else
#error No valid platform specified
#endif
            SR_CORE_POSTCONDITION(m_PlatformAPI.isValid(), "Failed to create platform API")

        }

    }

    CoreCommand::~CoreCommand() {
        if (m_Instance == this){
            if (m_GladInstances > 0){
                SR_CORE_WARNING("Syrius is terminated but some objects still depend on glad, glad will be terminated too!");
                m_GladInstances = 0;
                gladLoaderUnloadGL();
            }

            m_Instance = nullptr;
            SR_CORE_MESSAGE("Terminating Syrius")
        }
    }

    void CoreCommand::initGlad() {
        if (!m_GladInstances){
            int32 version = gladLoaderLoadGL();
#if defined(SR_CORE_DEBUG)
            if (version > 0){
                int32 major = GLAD_VERSION_MAJOR(version);
                int32 minor = GLAD_VERSION_MINOR(version);
                SR_CORE_MESSAGE("OpenGL initialized with version: %i.%i", major, minor)
                if (major < 4 || (major == 4 && minor < 6)){
                    SR_CORE_WARNING("OpenGL version is lower than 4.5, some features may not work")
                }
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#if defined(SR_COMPILER_MSVC)
                auto func = (GLDEBUGPROC) DebugMessageHandler::pushOpenGLMessageCallback;
                glDebugMessageCallback(func, nullptr);
#else
                glDebugMessageCallback(DebugMessageHandler::pushOpenGLMessageCallback, nullptr);
#endif

            }
            else {
                SR_CORE_EXCEPTION("Failed to initialize OpenGL");
            }

#endif

        }
        m_GladInstances++;
    }

    void CoreCommand::terminateGlad() {
        m_GladInstances--;
        if (!m_GladInstances){
            gladLoaderUnloadGL();
        }
    }

    void CoreCommand::initPlatformGlad(GlPlatformDesc* glDesc){
        SR_CORE_PRECONDITION(m_PlatformAPI.isValid(), "Cannot initialize platform glad, no platform API was created");

        m_PlatformAPI->initPlatformGlad(glDesc);
    }

    void CoreCommand::terminatePlatformGlad() {
        SR_CORE_PRECONDITION(m_PlatformAPI.isValid(), "Cannot initialize platform glad, no platform API was created");

        m_PlatformAPI->terminatePlatformGlad();
    }

    Time CoreCommand::getStartupTime() {
        return m_StartupTime;
    }

    Time CoreCommand::getElapsedTimeSinceStart() {
        Time currTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
        return currTime - m_StartupTime;
    }

    uint32 CoreCommand::getPrimaryScreenWidth() {
        return m_PlatformAPI->getPrimaryScreenWidth();
    }

    uint32 CoreCommand::getPrimaryScreenHeight() {
        return m_PlatformAPI->getPrimaryScreenHeight();
    }

    Resource<SyriusWindow> CoreCommand::createWindow(const WindowDesc &windowDesc) {
        SR_CORE_PRECONDITION(m_PlatformAPI.isValid(), "Cannot create window, no platform API was created");

        return m_PlatformAPI->createWindow(windowDesc, this);
    }

}