#include "../../../include/SyriusCore/Context/Context.hpp"

namespace Syrius{

    Context::Context(const ContextDesc& desc)
    : m_VerticalSync(false),
    m_Type(desc.m_API){

    }

    Context::~Context() {

    }

    SR_SUPPORTED_API Context::getType() const {
        return m_Type;
    }

    bool Context::isVerticalSyncEnabled() const {
        return m_VerticalSync;
    }

    VertexDescription *Context::createVertexDescription() {
        return new VertexDescription();
    }

}