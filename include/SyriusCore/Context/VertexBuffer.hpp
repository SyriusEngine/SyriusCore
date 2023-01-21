#pragma once

#include "Utils.hpp"
#include "../Utils/Resource.hpp"
#include "VertexDescription.hpp"

namespace Syrius{

    struct SR_CORE_API VertexBufferDesc{
        const void* data              = nullptr;
        uint32 count                  = 0;
        SR_BUFFER_TYPE type           = SR_BUFFER_DEFAULT;
        ResourceView<VertexDescription> layout;
    };

    class SR_CORE_API VertexBuffer{
    public:
        explicit VertexBuffer(const VertexBufferDesc& desc);

        virtual ~VertexBuffer();

        virtual void bind() = 0;

        virtual void setData(const void* data) = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] uint32 getSize() const;

        [[nodiscard]] uint32 getCount() const;

        [[nodiscard]] SR_BUFFER_TYPE getType() const;

        [[nodiscard]] const ResourceView<VertexDescription>& getVertexDescription() const;

    protected:
        uint32 m_Size;
        uint32 m_Count;
        ResourceView<VertexDescription> m_Layout;
        SR_BUFFER_TYPE m_Type;

    };

}

