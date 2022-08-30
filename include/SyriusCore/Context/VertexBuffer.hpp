#pragma once

#include "Utils.hpp"
#include "VertexDescription.hpp"

namespace Syrius{

    struct SR_API VertexBufferDesc{
        const void* m_Data = nullptr;
        uint32 m_Count = 0;
        SR_BUFFER_TYPE m_Type = SR_BUFFER_DEFAULT;
        VertexDescription* m_Layout = nullptr;
    };

    class SR_API VertexBuffer{
    public:
        explicit VertexBuffer(const VertexBufferDesc& desc);

        virtual ~VertexBuffer();

        virtual void bind() = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getSize() const;

        [[nodiscard]] uint32 getCount() const;

        [[nodiscard]] SR_BUFFER_TYPE getType() const;

        [[nodiscard]] VertexDescription* getVertexDescription() const;

    protected:
        uint32 m_Size;
        uint32 m_Count;
        VertexDescription* m_Layout;
        SR_BUFFER_TYPE m_Type;

    };

}

