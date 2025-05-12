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

        virtual void release() = 0;

        virtual void bind() = 0;

        virtual void setData(const void* data, u64 count) = 0;

        virtual void copyFrom(const ResourceView<IndexBuffer>& other) = 0;

        [[nodiscard]] virtual UP<UByte[]> getData() const = 0;

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] virtual Size getSize() const = 0;

        [[nodiscard]] virtual SR_BUFFER_USAGE getUsage() const = 0;

        [[nodiscard]] u64 getCount() const;

        [[nodiscard]] SR_TYPE getDataType() const;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        u64 m_Count;
        const SR_TYPE m_DataType;
    };
}