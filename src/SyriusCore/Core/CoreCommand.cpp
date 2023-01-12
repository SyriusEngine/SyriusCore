#include "CoreCommand.hpp"
#include "PlatformAPIWin32Impl.hpp"
#include "PlatformAPIX11Impl.hpp"

namespace Syrius{

    Time CoreCommand::m_StartupTime = 0;
    uint32 CoreCommand::m_GladInstances = 0;
    uint32 CoreCommand::m_CoreCommandInstances = 0;
    uint32 CoreCommand::m_VulkanInterfaces = 0;

    PlatformAPI* CoreCommand::m_PlatformAPI = nullptr;

    std::vector<SyriusWindow*> CoreCommand::m_WindowInstances;
    std::vector<Image*> CoreCommand::m_Images;


    void CoreCommand::init() {
        if (!m_CoreCommandInstances){
            m_StartupTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();

            SR_CORE_MESSAGE("Initializing Syrius")

#if defined(SR_CORE_PLATFORM_WIN64)
            m_PlatformAPI = new PlatformAPIWin32Impl();
#elif defined(SR_CORE_PLATFORM_LINUX)
            m_PlatformAPI = new PlatformAPIX11Impl();
#else
#error No PlatformAPI specified
#endif

        }
        m_CoreCommandInstances++;

        SR_CORE_POSTCONDITION(m_PlatformAPI != nullptr, "Failed to create platform API")
    }

    void CoreCommand::terminate() {
        for (auto img: m_Images) {
            delete img;
        }
        if (m_GladInstances > 0){
            SR_CORE_WARNING("Syrius is terminated but some objects still depend on glad, glad will be terminated too!");
            m_GladInstances = 0;
            gladLoaderUnloadGL();
        }


        SR_CORE_MESSAGE("Terminating Syrius")
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
        SR_CORE_PRECONDITION(m_PlatformAPI, "Cannot initialize platform glad, no platform API was created");

        m_PlatformAPI->initPlatformGlad(glDesc);
    }

    void CoreCommand::terminatePlatformGlad() {
        SR_CORE_PRECONDITION(m_PlatformAPI, "Cannot initialize platform glad, no platform API was created");

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

    SyriusWindow *CoreCommand::createWindow(const WindowDesc &windowDesc) {
        auto ptr = m_PlatformAPI->createWindow(windowDesc);
        m_WindowInstances.push_back(ptr);
        return ptr;
    }

    void CoreCommand::destroyWindow(SyriusWindow *window) {
        m_WindowInstances.erase(std::remove(m_WindowInstances.begin(), m_WindowInstances.end(), window), m_WindowInstances.end());
        delete window;
    }

    Image* CoreCommand::createImage(const std::string& fileName, bool flipOnLoad){
        auto ptr = new Image(fileName, flipOnLoad);
        m_Images.push_back(ptr);
        return ptr;
    }

    Image* CoreCommand::createImage(const ubyte* pixelData, int32 width, int32 height, int32 channelCount){
        auto ptr = new Image(pixelData, width, height, channelCount);
        m_Images.push_back(ptr);
        return ptr;
    }

    void CoreCommand::destroyImage(Image* image){
        m_Images.erase(std::remove(m_Images.begin(), m_Images.end(), image), m_Images.end());
        delete image;
    }
}