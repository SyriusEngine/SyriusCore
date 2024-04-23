#include "../../../include/SyriusCore/Context/VertexArray.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    VertexArray::VertexArray(const VertexArrayDesc &desc):
    m_VertexBuffer(desc.vertexBuffer),
    m_DrawMode(desc.drawMode) {

    }

    SR_DRAW_MODE VertexArray::getDrawMode() const {
        return m_DrawMode;
    }

    VertexArrayIndexed::VertexArrayIndexed(const VertexArrayDesc &desc):
    VertexArray(desc),
    m_IndexBuffer(desc.indexBuffer) {

    }
}