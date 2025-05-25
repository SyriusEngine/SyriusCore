#pragma once

#include "Utils.hpp"
#include "DeviceLimits.hpp"

namespace Syrius {

    struct SR_CORE_API ShaderStorageBufferDesc{
        std::string name;
        const void* data = nullptr;
        Size size = 0;
        SR_BUFFER_USAGE usage = SR_BUFFER_USAGE_DEFAULT;
        SR_SHADER_TYPE shaderStage = SR_SHADER_VERTEX;
    };

    class SR_CORE_API ShaderStorageBuffer {
    public:
        ShaderStorageBuffer(const ShaderStorageBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~ShaderStorageBuffer() = default;

        virtual void release() = 0;

        virtual void bind() = 0;

        virtual void bindShaderResource(u32 slot) = 0;

        virtual void setData(const void* data, Size size) = 0;

        virtual void copyFrom(const ResourceView<ShaderStorageBuffer>& other) = 0;

        [[nodiscard]] virtual UP<UByte[]> getData() const = 0;

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] virtual const std::string& getName() const = 0;

        [[nodiscard]] virtual Size getSize() const = 0;

        [[nodiscard]] virtual SR_BUFFER_USAGE getUsage() const = 0;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;
    };
}