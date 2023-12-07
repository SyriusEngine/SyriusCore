#pragma once

#include "Utils.hpp"
#include "../Utils/Resource.hpp"
#include "VertexLayout.hpp"

namespace Syrius{

    struct SR_CORE_API VertexBufferDesc{
        const void* data              = nullptr;
        uint32 count                  = 0;
        SR_BUFFER_USAGE usage           = SR_BUFFER_USAGE_DEFAULT;
        ResourceView<VertexLayout> layout;
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

        [[nodiscard]] SR_BUFFER_USAGE getType() const;

        [[nodiscard]] const ResourceView<VertexLayout>& getVertexDescription() const;

    protected:
        uint32 m_Size;
        uint32 m_Count;
        ResourceView<VertexLayout> m_Layout;
        SR_BUFFER_USAGE m_Type;

    };

}

