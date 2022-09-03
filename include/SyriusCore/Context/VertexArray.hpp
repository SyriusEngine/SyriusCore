#pragma once

#include "Utils.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "ShaderModule.hpp"

namespace Syrius{

    struct SR_API VertexArrayDesc{
        VertexBuffer* m_VertexBuffer    = nullptr;
        IndexBuffer* m_IndexBuffer      = nullptr;
        ShaderModule* m_VertexShader    = nullptr;
        SR_DRAW_TYPE m_DrawMode         = SR_DRAW_TRIANGLES;
    };

    class SR_API VertexArray{
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
        VertexBuffer* m_VertexBuffer;
        IndexBuffer* m_IndexBuffer;
    };

}

