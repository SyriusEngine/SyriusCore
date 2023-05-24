#include "../../../include/SyriusCore/Core/PlatformAPI.hpp"

#include "PlatformInclude.hpp"
#include "DebugMacros.hpp"

#include <utility>
#include <vulkan/vulkan.hpp>

namespace Syrius{

    VulkanPlatformDesc::VulkanPlatformDesc() {

#if defined(SR_CORE_DEBUG)
        addLayer("VK_LAYER_KHRONOS_validation");
        addExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    }

    uint32 VulkanPlatformDesc::getExtensionCount() const {
        return m_Extensions.size();
    }

    uint32 VulkanPlatformDesc::getLayerCount() const {
        return m_ValidationLayers.size();
    }

    const std::vector<std::string> &VulkanPlatformDesc::getExtensionNames() const {
        return m_Extensions;
    }

    const std::vector<std::string> &VulkanPlatformDesc::getLayerNames() const {
        return m_ValidationLayers;
    }

    void VulkanPlatformDesc::addExtension(const std::string &extensionName) {
        m_Extensions.emplace_back(extensionName);
    }

    void VulkanPlatformDesc::addLayer(const std::string &layerName) {
        m_ValidationLayers.emplace_back(layerName);
    }

    PlatformAPI* PlatformAPI::m_Instance = nullptr;

    PlatformAPI::PlatformAPI(VulkanPlatformDesc vpDesc) :
    m_GlVersion(0),
    m_GladInstances(0),
    m_PlatformGladInstances(0),
    m_VulkanPlatformDesc(std::move(vpDesc)){
        if (m_Instance){
            SR_CORE_EXCEPTION("PlatformAPI instance already exists")
        }
        else{
            m_Instance = this;
        }
    }

    PlatformAPI::~PlatformAPI() {
    }

    void PlatformAPI::initGlad() {
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

    void PlatformAPI::terminateGlad() {
        m_GladInstances--;
        if (!m_GladInstances){
            gladLoaderUnloadGL();
        }
    }

    int32 PlatformAPI::getGlVersion() const {
        return m_GlVersion;
    }

    const VulkanPlatformDesc &PlatformAPI::getVulkanPlatformDesc() const {
        return m_VulkanPlatformDesc;
    }


}