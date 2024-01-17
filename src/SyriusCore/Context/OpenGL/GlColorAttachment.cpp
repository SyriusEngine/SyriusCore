#include "GlColorAttachment.hpp"

namespace Syrius{

    GlColorAttachment::GlColorAttachment(uint32 framebufferID, uint32 attachmentID, const ColorAttachmentDesc &desc):
    ColorAttachment(desc),
    m_InternalFormat(getGlTextureInternalFormat(desc.format)),
    m_FrameBufferID(framebufferID),
    m_AttachmentID(attachmentID),
    m_TextureID(0),
    m_ChannelCount(0),
    m_GlDataType(getGlDataType(getTextureDataType(desc.format))){
        SR_CHANNEL_FORMAT baseFormat = getTextureChannelFormat(desc.format);
        m_GlFormat = getGlChannelType(baseFormat);
        m_ChannelCount = getChannelFormatCount(baseFormat);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        GLint supportedFormat = GL_FALSE;
        glGetInternalformativ(GL_TEXTURE_2D, m_InternalFormat, GL_INTERNALFORMAT_SUPPORTED, 1, &supportedFormat);
        if (supportedFormat == GL_TRUE){
            SR_CORE_OPENGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_GlFormat, m_GlDataType, nullptr));
            glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else{
            SR_CORE_EXCEPTION("OpenGL does not support a color attachment of usage: %i", desc.format);
        }
    }

    GlColorAttachment::~GlColorAttachment() {
        glDeleteTextures(1, &m_TextureID);
    }

    void GlColorAttachment::bind() {

    }

    void GlColorAttachment::bindShaderResource(uint32 slot) {
        glBindTextureUnit(slot, m_TextureID);
    }

    void GlColorAttachment::clear() {
        glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, m_AttachmentID, &m_ClearColor[0]);
    }

    void GlColorAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
        /*
         * This underlying line wont worke for some reason?
         * We use the old method for now (glBind spamming)
         */
        // glTextureStorage2D(m_TextureID, 1, m_InternalFormat, width, height);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_GlFormat, m_GlDataType, nullptr);
    }

    Resource<Image> GlColorAttachment::getData() {
        uint32 size = m_Width * m_Height * m_ChannelCount;
        auto data = Resource<uint8>(new uint8[size]);
        glGetTextureImage(m_TextureID, 0, m_GlFormat, GL_UNSIGNED_BYTE, size, data.get());
        ImageUI8Desc imgDesc;
        imgDesc.width = m_Width;
        imgDesc.height = m_Height;
        imgDesc.format = m_Format;
        imgDesc.data = data.get();
        return createImageUI8(imgDesc);
    }

    uint64 GlColorAttachment::getIdentifier() const {
        return m_TextureID;
    }


    GlDefaultColorAttachment::GlDefaultColorAttachment(const ColorAttachmentDesc &desc):
    ColorAttachment(desc){

    }

    GlDefaultColorAttachment::~GlDefaultColorAttachment() {
    }

    void GlDefaultColorAttachment::bind() {

    }

    void GlDefaultColorAttachment::bindShaderResource(uint32 slot) {
    }

    void GlDefaultColorAttachment::clear() {
       glClearNamedFramebufferfv(0, GL_COLOR, 0, m_ClearColor);
    }

    void GlDefaultColorAttachment::onResize(uint32 width, uint32 height) {
        m_Width = width;
        m_Height = height;
    }

    Resource<Image> GlDefaultColorAttachment::getData() {
        return {};
    }

    uint64 GlDefaultColorAttachment::getIdentifier() const {
        return 0;
    }


}