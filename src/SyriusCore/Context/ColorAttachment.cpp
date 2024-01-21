#include "../../../include/SyriusCore/Context/ColorAttachment.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    ColorAttachment::ColorAttachment(const ColorAttachmentDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    m_Width(desc.width),
    m_Height(desc.height),
    m_Format(desc.format),
    m_DeviceLimits(deviceLimits),
    m_ClearColor{desc.clearColor[0], desc.clearColor[1], desc.clearColor[2], desc.clearColor[3]},
    m_EnableShaderRead(desc.enableShaderRead){

    }

    ColorAttachment::~ColorAttachment() {

    }

    void ColorAttachment::setClearColor(float r, float g, float b, float a) {
        m_ClearColor[0] = r;
        m_ClearColor[1] = g;
        m_ClearColor[2] = b;
        m_ClearColor[3] = a;
    }

    uint32 ColorAttachment::getWidth() const {
        return m_Width;
    }

    uint32 ColorAttachment::getHeight() const {
        return m_Height;
    }

    SR_TEXTURE_FORMAT ColorAttachment::getFormat() const {
        return m_Format;
    }

    float* ColorAttachment::getClearColor(){
        return m_ClearColor;
    }

    bool ColorAttachment::shaderReadEnabled() const {
        return m_EnableShaderRead;
    }
}
