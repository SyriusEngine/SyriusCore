#pragma once

#include "Utils.hpp"
#include "VertexLayout.hpp"
#include "DeviceLimits.hpp"

namespace Syrius{

    struct SR_CORE_API VertexBufferDesc{
        const void* data            = nullptr;
        u64 count                   = 0;
        SR_BUFFER_USAGE usage       = SR_BUFFER_USAGE_DEFAULT;
        ResourceView<VertexLayout> layout;
    };

    class SR_CORE_API VertexBuffer{
    public:
        VertexBuffer(const VertexBufferDesc& desc, const UP<DeviceLimits>& deviceLimits);

        virtual ~VertexBuffer();

        /**
         * @brief Destroys any references to the buffer on the GPU.
         */
        virtual void release() = 0;

        /**
         * @brief Binds the buffer to the pipeline.
         */
        virtual void bind() = 0;

        /**
         * @brief Updates the vertices in the buffer. Starting from the
         * beginning of the buffer.
         * @param data New data to set.
         * @param count Amount of vertices to set.
         *
         * @warning This operation may not fully complete if the buffer is too small
         *          In that case, the buffer will be copied until the end, so not all
         *          vertices may be present.
         */
        virtual void setData(const void* data, u64 count) = 0;

        /**
         * @brief Copies the vertices from another buffer to this buffer.
         * @param other Vertex buffer to copy from.
         *
         * @warning This operation may not fully complete if this buffer is
         * smaller than the other buffer. In that case, the other buffer will be
         *          copied partially (until this buffer is full).
         */
        virtual void copyFrom(const ResourceView<VertexBuffer>& other) = 0;

        /**
         * @brief Returns the vertices stored in this buffer.
         * @return A byte array containing the vertices.
         */
        [[nodiscard]] virtual UP<UByte[]> getData() const = 0;

        /**
         * @brief Returns the ID of the buffer
         * @return u64 ID of the buffer (can be an object like in D3D11 or an ID
         * like in OpenGL)
         */
        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        /**
         * @brief Returns the size of the buffer.
         * @return Size in bytes.
         */
        [[nodiscard]] virtual Size getSize() const = 0;

        /**
         * @brief Returns the usage flags that this buffer was configured with.
         * @return SR_BUFFER_USAGE flags
         */
        [[nodiscard]] virtual SR_BUFFER_USAGE getUsage() const = 0;

        /**
         * @brief Returns the amount of individual vertices stored in this
         * buffer.
         * @return Vertex Count.
         */
        [[nodiscard]] u64 getCount() const;

        /**
         * @brief Returns the vertex layout of this buffer.
         * @return VertexLayout object.
         */
        [[nodiscard]] const ResourceView<VertexLayout>& getLayout() const;

    protected:
        const UP<DeviceLimits>& m_DeviceLimits;

        u64 m_Count = 0;
        ResourceView<VertexLayout> m_Layout;

    };

}
