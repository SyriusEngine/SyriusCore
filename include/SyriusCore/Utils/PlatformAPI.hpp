#pragma once

#include "SyriusCoreInclude.hpp"
#include "../Window/SyriusWindow.hpp"

namespace Syrius {

    class CoreCommand;

    class SR_CORE_API VulkanPlatformDesc{
    public:

        [[nodiscard]] uint32 getExtensionCount() const;

        [[nodiscard]] uint32 getLayerCount() const;

        [[nodiscard]] const std::vector<std::string>& getExtensionNames() const;

        [[nodiscard]] const std::vector<std::string>& getLayerNames() const;

    protected:

        VulkanPlatformDesc();

        void addExtension(const std::string& extensionName);

        void addLayer(const std::string& layerName);

    private:
        std::vector<std::string> m_Extensions;
        std::vector<std::string> m_ValidationLayers;

    };

    class SR_CORE_API PlatformAPI{
    public:
        virtual ~PlatformAPI();

        virtual uint32 getPrimaryScreenWidth() = 0;

        virtual uint32 getPrimaryScreenHeight() = 0;

        // virtual void initPlatformGlad(...) = 0; X11 and WIN32 obviously have different parameters, no idea how to handle this

        virtual void terminatePlatformGlad() = 0;

        virtual Resource<SyriusWindow> createWindow(const WindowDesc& windowDesc) = 0;

        void initGlad();

        void terminateGlad();

        [[nodiscard]] int32 getGlVersion() const;

        [[nodiscard]] const VulkanPlatformDesc& getVulkanPlatformDesc() const;

    protected:

        explicit PlatformAPI(VulkanPlatformDesc vpDesc);

    protected:
        int32 m_GlVersion;
        uint32 m_PlatformGladInstances;

        VulkanPlatformDesc m_VulkanPlatformDesc;

    private:
        static PlatformAPI* m_Instance;

        uint32 m_GladInstances;
    };

}



