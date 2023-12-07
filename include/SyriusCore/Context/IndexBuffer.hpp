#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_CORE_API IndexBufferDesc{
        const void* data      = nullptr;
        uint32 count          = 0;
        SR_BUFFER_USAGE usage   = SR_BUFFER_USAGE_DEFAULT;
        SR_TYPE dataType = SR_UINT32;
    };

    class SR_CORE_API IndexBuffer{
    public:
        explicit IndexBuffer(const IndexBufferDesc& desc);

        virtual ~IndexBuffer();

        virtual void bind() = 0;

        virtual void setData(const void* data) = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getSize() const;

        [[nodiscard]] uint32 getCount() const;

        [[nodiscard]] SR_BUFFER_USAGE getType() const;

        [[nodiscard]] SR_TYPE getDataType() const;

    protected:
        uint32 m_Size;
        uint32 m_Count;
        SR_BUFFER_USAGE m_Type;
        SR_TYPE m_DataType;
    };
}