#include "../../../../include/SyriusCore/Context/ColorAttachment.hpp"

namespace Syrius{

    ColorAttachment::ColorAttachment(const ColorAttachmentDesc &desc)
    : m_Width(desc.m_Width),
    m_Height(desc.m_Height),
    m_Format(desc.m_Format),
    m_ClearColor{desc.m_ClearColor[0], desc.m_ClearColor[1], desc.m_ClearColor[2], desc.m_ClearColor[3]}{

    }

    ColorAttachment::~ColorAttachment() {

    }

    uint32 ColorAttachment::getWidth() const {
        return m_Width;
    }

    uint32 ColorAttachment::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_DATA_FORMAT ColorAttachment::getFormat() const {
        return m_Format;
    }

    const float *ColorAttachment::getClearColor() const {
        return m_ClearColor;
    }

    void ColorAttachment::setClearColor(float r, float g, float b, float a) {
        m_ClearColor[0] = r;
        m_ClearColor[1] = g;
        m_ClearColor[2] = b;
        m_ClearColor[3] = a;
    }
}