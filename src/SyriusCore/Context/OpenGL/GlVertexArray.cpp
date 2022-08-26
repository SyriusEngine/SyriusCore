#include "GlVertexArray.hpp"

namespace Syrius{

    GlVertexArray::GlVertexArray(const VertexArrayDesc &desc)
    : VertexArray(desc),
    m_GlDrawMode(getGlDrawType(desc.m_DrawMode)),
    m_RendererID(0),
    m_Offset(0),
    m_VertexAttribPointer(0) {

        glGenVertexArrays(1, &m_RendererID);
        glBindVertexArray(m_RendererID);
        m_VertexBuffer->bind();

        for (const auto& element: m_VertexBuffer->getVertexDescription()->getAttributes()){
            glEnableVertexAttribArray(m_VertexAttribPointer);
            SR_DATA_TYPE dataType = getScalarDataType(element.m_Type);
            glVertexAttribPointer(m_VertexAttribPointer,
                                  element.m_ElementCount,
                                  getGlDataType(dataType),
                                  false,
                                  static_cast<int>(m_VertexBuffer->getVertexDescription()->getStride()),
                                  reinterpret_cast<const void*>(m_Offset));
            m_VertexAttribPointer++;
            m_Offset += getTypeSize(element.m_Type);
        }
    }

    GlVertexArray::~GlVertexArray() {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void GlVertexArray::bind() {
        glBindVertexArray(m_RendererID);
    }

    void GlVertexArray::unbind() {
        glBindVertexArray(0);
    }

    void GlVertexArray::drawBuffers() {
        glBindVertexArray(m_RendererID);
        glDrawArrays(m_GlDrawMode, 0, m_VertexBuffer->getCount());
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

        glBindVertexArray(m_RendererID);
        desc.m_IndexBuffer->bind();
    }

    GlVertexArrayIndexed::~GlVertexArrayIndexed() = default;

    void GlVertexArrayIndexed::drawBuffers() {
        glBindVertexArray(m_RendererID);
        glDrawElements(m_GlDrawMode, m_IndexBuffer->getCount(), m_IndexDataType, nullptr);
        glBindVertexArray(0);
    }

}
