#include "../../../../include/SyriusCore/Context/ColorAttachment.hpp"

namespace Syrius{

    ColorAttachment::ColorAttachment(const ColorAttachmentDesc &desc)
    : m_Width(desc.m_Width),
    m_Height(desc.m_Height),
    m_Format(desc.m_Format){

    }

    ColorAttachment::~ColorAttachment() {

    }

    uint32 ColorAttachment::getWidth() const {
        return m_Width;
    }

    uint32 ColorAttachment::getHeight() const {
        return m_Height;
    }
}