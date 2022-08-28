#include "GlVertexArray.hpp"
#include "GlVertexBuffer.hpp"

namespace Syrius{

    GlVertexArray::GlVertexArray(const VertexArrayDesc &desc)
    : VertexArray(desc),
    m_GlDrawMode(getGlDrawType(desc.m_DrawMode)),
    m_ArrayID(0) {
        glCreateVertexArrays(1, &m_ArrayID);

        uint32 offset = 0;
        uint32 attribPtr = 0;

        for (const auto& element: m_VertexBuffer->getVertexDescription()->getAttributes()) {
            glEnableVertexArrayAttrib(m_ArrayID, attribPtr);
            glVertexArrayAttribBinding(m_ArrayID, attribPtr, 0);
            SR_DATA_TYPE dataType = getScalarDataType(element.m_Type);
            glVertexArrayAttribFormat(m_ArrayID, attribPtr, element.m_ElementCount, getGlDataType(dataType), GL_FALSE,
                                      offset);

            attribPtr++;
            offset += getTypeSize(dataType) * element.m_ElementCount;
        }

        auto vboID = m_VertexBuffer->getIdentifier();
        glVertexArrayVertexBuffer(m_ArrayID, 0, vboID, 0, offset);

    }

    GlVertexArray::~GlVertexArray() {
        glDeleteVertexArrays(1, &m_ArrayID);
    }

    void GlVertexArray::bind() {
        glBindVertexArray(m_ArrayID);
    }

    void GlVertexArray::unbind() {
        glBindVertexArray(0);
    }

    void GlVertexArray::drawBuffers() {
        glBindVertexArray(m_ArrayID);
        SR_OPENGL_CALL(glDrawArrays(m_GlDrawMode, 0, m_VertexBuffer->getCount()));
        glBindVertexArray(0);
    }

    void GlVertexArray::drawBuffersInstanced(uint32 instanceCount) {
        glBindVertexArray(m_ArrayID);
        SR_OPENGL_CALL(glDrawArraysInstanced(m_GlDrawMode, 0, m_VertexBuffer->getCount(), instanceCount));
        glBindVertexArray(0);
    }

    void GlVertexArray::setDrawMode(SR_DRAW_TYPE drawMode) {
        m_DrawMode = drawMode;
        m_GlDrawMode = getGlDrawType(drawMode);
    }


    GlVertexArrayIndexed::GlVertexArrayIndexed(const VertexArrayDesc &desc)
    : GlVertexArray(desc),
    m_IndexDataType(getGlDataType(desc.m_IndexBuffer->getDataType())){
        SR_CORE_PRECONDITION(desc.m_IndexBuffer != nullptr, "IndexBuffer is nullptr");

        auto iboID = desc.m_IndexBuffer->getIdentifier();
        glVertexArrayElementBuffer(m_ArrayID, iboID);
    }

    GlVertexArrayIndexed::~GlVertexArrayIndexed() = default;

    void GlVertexArrayIndexed::drawBuffers() {
        glBindVertexArray(m_ArrayID);
        SR_OPENGL_CALL(glDrawElements(m_GlDrawMode, m_IndexBuffer->getCount(), m_IndexDataType, nullptr));
        glBindVertexArray(0);
    }

    void GlVertexArrayIndexed::drawBuffersInstanced(uint32 instanceCount) {
        glBindVertexArray(m_ArrayID);
        SR_OPENGL_CALL(glDrawElementsInstanced(m_GlDrawMode, m_IndexBuffer->getCount(), m_IndexDataType, nullptr, instanceCount));
        glBindVertexArray(0);
    }

}
