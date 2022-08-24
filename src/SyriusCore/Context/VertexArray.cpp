#include "../../../../include/SyriusCore/Context/VertexArray.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    VertexArray::VertexArray(const VertexArrayDesc &desc)
    : m_VertexBuffer(desc.m_VertexBuffer),
    m_IndexBuffer(desc.m_IndexBuffer),
    m_DrawMode(desc.m_DrawMode) {

    }

    VertexArray::~VertexArray() {

    }

    SR_DRAW_TYPE VertexArray::getDrawMode() const {
        return m_DrawMode;
    }
}