#include "../../../../include/SyriusCore/Context/ColorAttachment.hpp"

namespace Syrius{

    ColorAttachment::ColorAttachment(const ColorAttachmentDesc &desc)
    : m_Width(desc.m_Width),
    m_Height(desc.m_Height),
    m_Format(desc.m_Format){

    }

    ColorAttachment::~ColorAttachment() {

    }
}