#pragma once

#include "Utils.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API ShaderStorageBufferDesc{
        std::string name;
        const void* data = nullptr;
        uint32 size = 0;
        SR_BUFFER_USAGE usage = SR_BUFFER_USAGE_DEFAULT;
        SR_SHADER_TYPE shaderStage = SR_SHADER_VERTEX;
    };

    class SR_CORE_API ShaderStorageBuffer{
    public:
        ShaderStorageBuffer(const ShaderStorageBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        virtual ~ShaderStorageBuffer();

        virtual void bind() = 0;

        virtual void bindShaderResource(uint32 slot) = 0;

        virtual void setData(const void* data, uint32 size) = 0;

        [[nodiscard]] virtual Resource<byte[]> getData() const = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] uint32 getSize() const;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const;

        [[nodiscard]] SR_SHADER_TYPE getShaderStage() const;

    protected:
        const Resource<DeviceLimits>& m_DeviceLimits;

        const std::string m_Name;
        const uint32 m_Size;
        const SR_BUFFER_USAGE m_Usage;
        const SR_SHADER_TYPE m_ShaderStage;
    };

}