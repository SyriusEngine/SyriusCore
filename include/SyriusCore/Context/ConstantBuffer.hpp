#pragma once

#include "Utils.hpp"
#include "Shader.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API ConstantBufferDesc{
        std::string name;
        u32 size                 = 0;
        const void* data            = nullptr;
        SR_BUFFER_USAGE usage       = SR_BUFFER_USAGE_DEFAULT;
        SR_SHADER_TYPE shaderStage  = SR_SHADER_VERTEX;
    };

    class SR_CORE_API ConstantBuffer{
    public:
        ConstantBuffer(const ConstantBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~ConstantBuffer() = default;

        virtual void bind(u32 slot) = 0;

        virtual void setData(const void* data, u64 size) = 0;

        virtual void copyFrom(const ResourceView<ConstantBuffer>& other) = 0;

        [[nodiscard]] virtual UP<UByte[]> getData() const = 0;

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] u32 getSize() const;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        u32 m_Size;
        SR_BUFFER_USAGE m_Usage;
        std::string m_Name;
    };

}

