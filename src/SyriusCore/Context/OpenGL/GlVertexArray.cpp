#include "GlVertexArray.hpp"
#include "GlVertexBuffer.hpp"

namespace Syrius{

    GlVertexArray::GlVertexArray(const VertexArrayDesc &desc):
    VertexArray(desc),
    m_GlDrawMode(getGlDrawType(desc.drawMode)),
    m_ArrayID(0) {
        glCreateVertexArrays(1, &m_ArrayID);

        u32 offset = 0;
        u32 attribPtr = 0;

        for (const auto& element: m_VertexBuffer->getLayout()->getAttributes()) {
            glEnableVertexArrayAttrib(m_ArrayID, attribPtr);
            glVertexArrayAttribBinding(m_ArrayID, attribPtr, 0);
            SR_TYPE dataType = getScalarType(element.type);
            glVertexArrayAttribFormat(m_ArrayID, attribPtr, element.elementCount, getGlDataType(dataType), GL_FALSE,
                                      offset);

            attribPtr++;
            offset += element.size;
        }

        auto vboID = m_VertexBuffer->getIdentifier();
        glVertexArrayVertexBuffer(m_ArrayID, 0, vboID, 0, offset);
    }

    GlVertexArray::~GlVertexArray() {
        glDeleteVertexArrays(1, &m_ArrayID);
    }

    void GlVertexArray::bind() {
        /*
         * Thanks to DSA we don't need to bind the vertex buffer.
         * But in debug mode, we explicitly bind these since RenderDoc gives us a bunch of Incorrect API usage warnings.
         */
        #if defined(SR_DEBUG)
        m_VertexBuffer->bind();
        #endif
        glBindVertexArray(m_ArrayID);
    }

    void GlVertexArray::unbind() {
        glBindVertexArray(0);
    }

    void GlVertexArray::drawBuffers() {
        GlVertexArray::bind();
        SR_CORE_OPENGL_CALL(glDrawArrays(m_GlDrawMode, 0, m_VertexBuffer->getCount()));
        glBindVertexArray(0);
    }

    void GlVertexArray::drawBuffersInstanced(u32 instanceCount) {
        glBindVertexArray(m_ArrayID);
        SR_CORE_OPENGL_CALL(glDrawArraysInstanced(m_GlDrawMode, 0, m_VertexBuffer->getCount(), instanceCount));
        glBindVertexArray(0);
    }

    void GlVertexArray::setDrawMode(SR_DRAW_MODE drawMode) {
        m_DrawMode = drawMode;
        m_GlDrawMode = getGlDrawType(drawMode);
    }


    GlVertexArrayIndexed::GlVertexArrayIndexed(const VertexArrayDesc &desc):
    VertexArrayIndexed(desc),
    m_GlDrawMode(getGlDrawType(desc.drawMode)),
    m_IndexDataType(getGlDataType(desc.indexBuffer->getDataType())),
    m_ArrayID(0){
        SR_PRECONDITION(desc.indexBuffer != nullptr, "IndexBuffer is nullptr");

        glCreateVertexArrays(1, &m_ArrayID);

        u32 offset = 0;
        u32 attribPtr = 0;

        for (const auto& element: m_VertexBuffer->getLayout()->getAttributes()) {
            glEnableVertexArrayAttrib(m_ArrayID, attribPtr);
            glVertexArrayAttribBinding(m_ArrayID, attribPtr, 0);
            SR_TYPE dataType = getScalarType(element.type);
            glVertexArrayAttribFormat(m_ArrayID, attribPtr, element.elementCount, getGlDataType(dataType), GL_FALSE,
                                      offset);

            attribPtr++;
            offset += element.size;
        }

        auto vboID = m_VertexBuffer->getIdentifier();
        glVertexArrayVertexBuffer(m_ArrayID, 0, vboID, 0, offset);

        auto iboID = desc.indexBuffer->getIdentifier();
        glVertexArrayElementBuffer(m_ArrayID, iboID);
    }

    GlVertexArrayIndexed::~GlVertexArrayIndexed() {
        glDeleteVertexArrays(1, &m_ArrayID);
    }

    void GlVertexArrayIndexed::bind() {
        /*
         * Thanks to DSA we don't need to bind the vertex buffer.
         * But in debug mode, we explicitly bind these since RenderDoc gives us a bunch of Incorrect API usage warnings.
         */
        #if defined(SR_DEBUG)
        m_VertexBuffer->bind();
        m_IndexBuffer->bind();
        #endif
        glBindVertexArray(m_ArrayID);
    }

    void GlVertexArrayIndexed::unbind() {
        glBindVertexArray(0);
    }

    void GlVertexArrayIndexed::drawBuffers() {
        GlVertexArrayIndexed::bind();
        SR_CORE_OPENGL_CALL(glDrawElements(m_GlDrawMode, m_IndexBuffer->getCount(), m_IndexDataType, nullptr));
        glBindVertexArray(0);
    }

    void GlVertexArrayIndexed::drawBuffersInstanced(u32 instanceCount) {
        GlVertexArrayIndexed::bind();
        SR_CORE_OPENGL_CALL(glDrawElementsInstanced(m_GlDrawMode, m_IndexBuffer->getCount(), m_IndexDataType, nullptr, instanceCount));
        glBindVertexArray(0);
    }

    void GlVertexArrayIndexed::setDrawMode(SR_DRAW_MODE drawMode) {
        m_DrawMode = drawMode;
        m_GlDrawMode = getGlDrawType(drawMode);
    }

}
