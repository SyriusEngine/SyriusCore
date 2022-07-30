#pragma once

#include "SyriusCoreInclude.hpp"
#include "../Window/SyriusWindow.hpp"

namespace Syrius {

    class SR_API VulkanPlatformDesc{
    public:

        [[nodiscard]] uint32 getExtensionCount() const;

        [[nodiscard]] uint32 getLayerCount() const;

        [[nodiscard]] const std::vector<std::string>& getExtensionNames() const;

        [[nodiscard]] const std::vector<std::string>& getLayerNames() const;

    protected:

        VulkanPlatformDesc();

        void addRequiredExtension(const std::string& extensionName);

        void addRequiredLayer(const std::string& layerName);

    private:
        std::vector<std::string> m_Extensions;
        std::vector<std::string> m_ValidationLayers;

    };

    struct SR_API GlPlatformDesc{
        GlPlatformDesc() = default;

        virtual ~GlPlatformDesc() = default;

    };

    class SR_API PlatformAPI{
    public:
        virtual ~PlatformAPI();

        virtual uint32 getPrimaryScreenWidth() = 0;

        virtual uint32 getPrimaryScreenHeight() = 0;

        virtual void initPlatformGlad(GlPlatformDesc* glDesc) = 0;

        virtual void terminatePlatformGlad() = 0;

        virtual SyriusWindow* createWindow(const WindowDesc& windowDesc) = 0;

        [[nodiscard]] int32 getGlVersion() const;

        [[nodiscard]] const VulkanPlatformDesc& getVulkanPlatformDesc() const;

    protected:

        explicit PlatformAPI(VulkanPlatformDesc vpDesc);

    protected:
        int32 m_GlVersion;
        uint32 m_PlatformGladInstances;

        VulkanPlatformDesc m_VulkanPlatformDesc;
    };

}



