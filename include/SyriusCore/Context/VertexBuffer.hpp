#pragma once

#include "Utils.hpp"
#include "VertexLayout.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API VertexBufferDesc{
        const void* data              = nullptr;
        u32 count                  = 0;
        SR_BUFFER_USAGE usage           = SR_BUFFER_USAGE_DEFAULT;
        ResourceView<VertexLayout> layout;
    };

    class SR_CORE_API VertexBuffer{
    public:
        VertexBuffer(const VertexBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~VertexBuffer();

        virtual void bind() = 0;

        virtual void setData(const void* data, u32 size) = 0;

        virtual void copyFrom(const ResourceView<VertexBuffer>& other) = 0;

        [[nodiscard]] virtual UP<UByte[]> getData() const = 0;

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] u32 getSize() const;

        [[nodiscard]] u32 getCount() const;

        [[nodiscard]] SR_BUFFER_USAGE getUsage() const;

        [[nodiscard]] const ResourceView<VertexLayout>& getLayout() const;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        u32 m_Size;
        u32 m_Count;
        ResourceView<VertexLayout> m_Layout;
        const SR_BUFFER_USAGE m_Usage;

    };

}

