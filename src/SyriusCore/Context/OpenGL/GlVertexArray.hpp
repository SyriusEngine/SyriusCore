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

        void setDrawMode(SR_DRAW_TYPE drawMode) override;

    protected:
        uint32 m_ArrayID;
        GLenum m_GlDrawMode;

    };

    class GlVertexArrayIndexed : public GlVertexArray{
    public:
        explicit GlVertexArrayIndexed(const VertexArrayDesc& desc);

        ~GlVertexArrayIndexed() override;

        void drawBuffers() override;

    private:
        GLenum m_IndexDataType;
    };
}

