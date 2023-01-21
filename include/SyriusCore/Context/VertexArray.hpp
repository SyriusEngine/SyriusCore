#pragma once

#include "Utils.hpp"
#include "../Utils/Resource.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "ShaderModule.hpp"

namespace Syrius{

    struct SR_CORE_API VertexArrayDesc{
        ResourceView<VertexBuffer> vertexBuffer;
        ResourceView<IndexBuffer> indexBuffer;
        ResourceView<ShaderModule> vertexShader;
        SR_DRAW_TYPE drawMode         = SR_DRAW_TRIANGLES;
    };

    class SR_CORE_API VertexArray{
    public:
        explicit VertexArray(const VertexArrayDesc& desc);

        virtual ~VertexArray();

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual void setDrawMode(SR_DRAW_TYPE drawMode) = 0;

        virtual void drawBuffers() = 0;

        virtual void drawBuffersInstanced(uint32 instanceCount) = 0;

        [[nodiscard]] SR_DRAW_TYPE getDrawMode() const;

    protected:
        SR_DRAW_TYPE m_DrawMode;
        ResourceView<VertexBuffer> m_VertexBuffer;
        ResourceView<IndexBuffer> m_IndexBuffer;
    };

}

