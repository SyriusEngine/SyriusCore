#pragma once

#include "Utils.hpp"
#include "Shader.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API ConstantBufferDesc{
        std::string name;
        Size size                   = 0;
        Size count                  = 0 ;
        const void* data            = nullptr;
        SR_BUFFER_USAGE usage       = SR_BUFFER_USAGE_DEFAULT;
        SR_SHADER_TYPE shaderStage  = SR_SHADER_VERTEX;
    };

    class SR_CORE_API ConstantBuffer{
    public:
        ConstantBuffer(const ConstantBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~ConstantBuffer() = default;

        virtual void release() = 0;

        virtual void bind() = 0;

        virtual void bindShaderResource(u32 slot) = 0;

        virtual void setData(const void* data, Size size) = 0;

        virtual void copyFrom(const ResourceView<ConstantBuffer>& other) = 0;

        [[nodiscard]] virtual UP<UByte[]> getData() const = 0;

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] virtual const std::string& getName() const = 0;

        [[nodiscard]] virtual Size getSize() const = 0;

        [[nodiscard]] virtual SR_BUFFER_USAGE getUsage() const = 0;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;
    };

}

