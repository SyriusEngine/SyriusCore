#pragma once

#include "Utils.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API IndexBufferDesc{
        const void* data      = nullptr;
        u32 count          = 0;
        SR_BUFFER_USAGE usage   = SR_BUFFER_USAGE_DEFAULT;
        SR_TYPE dataType = SR_UINT32;
    };

    class SR_CORE_API IndexBuffer{
    public:
        IndexBuffer(const IndexBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~IndexBuffer();

        virtual void bind() = 0;

        virtual void setData(const void* data, u32 count) = 0;

        virtual void copyFrom(const ResourceView<IndexBuffer>& other) = 0;

        [[nodiscard]] virtual UP<UByte[]> getData() const = 0;

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] u32 getSize() const;

        [[nodiscard]] u32 getCount() const;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const;

        [[nodiscard]] SR_TYPE getDataType() const;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        u32 m_Size;
        u32 m_Count;
        const SR_BUFFER_USAGE m_Usage;
        const SR_TYPE m_DataType;
    };
}