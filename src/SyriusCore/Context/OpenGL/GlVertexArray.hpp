#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/VertexArray.hpp"

namespace Syrius{

    class GlVertexArray : public VertexArray{
    public:
        explicit GlVertexArray(const VertexArrayDesc& desc);

        ~GlVertexArray() override;

        void bind() override;

        void unbind() override;

        void drawBuffers() override;

        void drawBuffersInstanced(u32 instanceCount) override;

        void setDrawMode(SR_DRAW_MODE drawMode) override;

    private:
        u32 m_ArrayID;
        GLenum m_GlDrawMode;

    };

    class GlVertexArrayIndexed : public VertexArrayIndexed{
    public:
        explicit GlVertexArrayIndexed(const VertexArrayDesc& desc);

        ~GlVertexArrayIndexed() override;

        void bind() override;

        void unbind() override;

        void drawBuffers() override;

        void drawBuffersInstanced(u32 instanceCount) override;

        void setDrawMode(SR_DRAW_MODE drawMode) override;

    private:
        u32 m_ArrayID;
        GLenum m_GlDrawMode;
        GLenum m_IndexDataType;
    };
}

