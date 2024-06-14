#pragma once

#include "Utils.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API IndexBufferDesc{
        const void* data      = nullptr;
        uint32 count          = 0;
        SR_BUFFER_USAGE usage   = SR_BUFFER_USAGE_DEFAULT;
        SR_TYPE dataType = SR_UINT32;
    };

    class SR_CORE_API IndexBuffer{
    public:
        IndexBuffer(const IndexBufferDesc& desc, const Resource<DeviceLimits>& deviceLimits);

        virtual ~IndexBuffer();

        virtual void bind() = 0;

        virtual void setData(const void* data, uint32 count) = 0;

        virtual void copyFrom(const ResourceView<IndexBuffer>& other) = 0;

        [[nodiscard]] virtual Resource<ubyte[]> getData() const = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getSize() const;

        [[nodiscard]] uint32 getCount() const;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const;

        [[nodiscard]] SR_TYPE getDataType() const;

    protected:
        const Resource<DeviceLimits>& m_DeviceLimits;

        uint32 m_Size;
        uint32 m_Count;
        const SR_BUFFER_USAGE m_Usage;
        const SR_TYPE m_DataType;
    };
}