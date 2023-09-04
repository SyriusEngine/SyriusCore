#include "../../../include/SyriusCore/Context/VertexArray.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    VertexArray::VertexArray(const VertexArrayDesc &desc)
    : m_VertexBuffer(desc.vertexBuffer),
    m_IndexBuffer(desc.indexBuffer),
    m_DrawMode(desc.drawMode) {

    }

    VertexArray::~VertexArray() {

    }

    SR_DRAW_TYPE VertexArray::getDrawMode() const {
        return m_DrawMode;
    }
}