#include "GlCubeColorAttachment.hpp"

namespace Syrius{

    GlCubeColorAttachment::GlCubeColorAttachment(const CubeColorAttachmentDesc &desc, const Resource<DeviceLimits> &deviceLimits, uint32 framebufferID, uint32 baseAttachmentID):
    CubeColorAttachment(desc, deviceLimits),
    m_FrameBufferID(framebufferID),
    m_BaseAttachmentID(baseAttachmentID),
    m_TextureID(0),
    m_InternalFormat(getGlTextureInternalFormat(desc.format)),
    m_GlDataType(getGlDataType(getTextureDataType(desc.format))){
        SR_CHANNEL_FORMAT baseFormat = getTextureChannelFormat(desc.format);
        m_GlFormat = getGlChannelType(baseFormat);

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        for(uint32 i = 0; i < 6; i++){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_GlFormat, m_GlDataType, nullptr);
        }
        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    GlCubeColorAttachment::~GlCubeColorAttachment() = default;

    void GlCubeColorAttachment::bind() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    }

    void GlCubeColorAttachment::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[GlCubeColorAttachment]: Supplied slot (%i) is greater than the device number of texture slots (%i)", slot, m_DeviceLimits->getMaxTextureSlots());

        glBindTextureUnit(slot, m_TextureID);
    }

    void GlCubeColorAttachment::clear() {
        for (uint32 i = 0; i < 6; i++){
            glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, m_BaseAttachmentID + i, m_ClearColor.data());
        }
    }

    void GlCubeColorAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        for(uint32 i = 0; i < 6; i++){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_GlFormat, m_GlDataType, nullptr);
        }
    }

    uint64 GlCubeColorAttachment::getIdentifier() const {
        return m_TextureID;
    }
}
