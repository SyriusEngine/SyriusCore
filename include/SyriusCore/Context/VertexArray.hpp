#pragma once

#include "Utils.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "ShaderModule.hpp"

namespace Syrius{

    struct SR_CORE_API VertexArrayDesc{
        ResourceView<VertexBuffer> vertexBuffer;
        ResourceView<IndexBuffer> indexBuffer;
        ResourceView<ShaderModule> vertexShader;
        SR_DRAW_MODE drawMode         = SR_DRAW_TRIANGLES;
    };

    class SR_CORE_API VertexArray{
    public:
        explicit VertexArray(const VertexArrayDesc& desc);

        virtual ~VertexArray() = default;

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual void setDrawMode(SR_DRAW_MODE drawMode) = 0;

        virtual void drawBuffers() = 0;

        virtual void drawBuffersInstanced(u32 instanceCount) = 0;

        [[nodiscard]] SR_DRAW_MODE getDrawMode() const;

    protected:
        SR_DRAW_MODE m_DrawMode;
        ResourceView<VertexBuffer> m_VertexBuffer;
    };

    class SR_CORE_API VertexArrayIndexed: public VertexArray{
    public:
        explicit VertexArrayIndexed(const VertexArrayDesc& desc);

        ~VertexArrayIndexed() override = default;

    protected:
        ResourceView<IndexBuffer> m_IndexBuffer;
    };

}

