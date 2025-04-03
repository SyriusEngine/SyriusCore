#include "GlColorAttachment.hpp"

namespace Syrius{

    GlColorAttachment::GlColorAttachment(const ColorAttachmentDesc &desc, const UP<DeviceLimits>& deviceLimits, u32 framebufferID, u32 attachmentID):
    ColorAttachment(desc, deviceLimits),
    m_FrameBufferID(framebufferID),
    m_AttachmentID(attachmentID),
    m_TextureID(0),
    m_GlChannelFormat(getGlChannelFormat(desc.format)),
    m_GlInternalFormat(getGlTextureFormat(desc.format)),
    m_GlDataType(getGlTextureDataType(desc.format)){
        SR_PRECONDITION(m_DeviceLimits->texture2DFormatSupported(desc.format), "GlColorAttachment", "Texture format {} is not supported by the device", desc.format);

//        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
//        glBindTexture(GL_TEXTURE_2D, m_TextureID);
//
//        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_GlFormat, m_GlDataType, nullptr);
//        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, m_GlInternalFormat, m_Width, m_Height);

        glNamedFramebufferTexture(m_FrameBufferID, GL_COLOR_ATTACHMENT0 + m_AttachmentID, m_TextureID, 0);
    }

    GlColorAttachment::~GlColorAttachment() {
        glDeleteTextures(1, &m_TextureID);
    }

    void GlColorAttachment::bind() {

    }

    void GlColorAttachment::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[Texture2D]: Supplied slot {} is greater than the device number of texture slots {}", slot, m_DeviceLimits->getMaxTextureSlots());

        glBindTextureUnit(slot, m_TextureID);
    }

    void GlColorAttachment::clear() {
        glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, m_AttachmentID, &m_ClearColor[0]);
    }

    void GlColorAttachment::onResize(u32 width, u32 height) {
        m_Width = width;
        m_Height = height;
        /*
         * This underlying line wont work for some reason?
         * We use the old method for now (glBind spamming)
         */
        glTextureStorage2D(m_TextureID, 1, m_GlInternalFormat, width, height);
        //glBindTexture(GL_TEXTURE_2D, m_TextureID);
        //glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_GlFormat, m_GlDataType, nullptr);
    }

    GLenum getGlReadPixelFormat(GLenum channelFormat, GLenum dataType){
        if (dataType == GL_UNSIGNED_BYTE or dataType == GL_BYTE or
            dataType == GL_UNSIGNED_SHORT or dataType == GL_SHORT or
            dataType == GL_UNSIGNED_INT or dataType == GL_INT){
            switch (channelFormat) {
                case GL_RED: return GL_RED_INTEGER;
                case GL_RG: return GL_RG_INTEGER;
                case GL_RGB: return GL_RGB_INTEGER;
                case GL_RGBA: return GL_RGBA_INTEGER;
                default: SR_LOG_THROW("GlColorAttachment", "Invalid channel format {}", channelFormat);
            }
        }
        return channelFormat;
    }

    UP<Image> GlColorAttachment::getData() {
        u32 size = m_Width * m_Height * getBytesPerPixel(m_Format);
        ImageDesc imgDesc;
        imgDesc.width = m_Width;
        imgDesc.height = m_Height;
        imgDesc.format = m_Format;
        imgDesc.data = nullptr;
        auto img =  createImage(imgDesc);

        // use a pbo to read the data from the texture
        GLuint pboID;
        glCreateBuffers(1, &pboID);
        glNamedBufferData(pboID, size, nullptr, GL_STATIC_READ);

        // bind the pbo
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pboID);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + m_AttachmentID);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        // copy the data from the texture to the pbo
        glReadPixels(0, 0, m_Width, m_Height, getGlReadPixelFormat(m_GlChannelFormat, m_GlDataType), m_GlDataType, nullptr);

        // map the pbo
        auto pBuffer = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);

        if (!pBuffer) {
            SR_LOG_WARNING("GlColorAttachment", "Failed to map pixel buffer object {}", pboID)
            return std::move(img);
        }

        memcpy(img->getData(), pBuffer, size);
        auto retVal = glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        if (retVal == GL_FALSE) {
            SR_LOG_WARNING("GlColorAttachment", "Failed to unmap pixel buffer object {}", pboID)
        }

        // unbind the pbo
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        glDeleteBuffers(1, &pboID);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return std::move(img);
    }

    u64 GlColorAttachment::getIdentifier() const {
        return m_TextureID;
    }


    GlDefaultColorAttachment::GlDefaultColorAttachment(const ColorAttachmentDesc &desc, const UP<DeviceLimits>& deviceLimits):
    ColorAttachment(desc, deviceLimits){

    }

    GlDefaultColorAttachment::~GlDefaultColorAttachment() = default;

    void GlDefaultColorAttachment::bind() {

    }

    void GlDefaultColorAttachment::bindShaderResource(u32 slot) {
        SR_LOG_WARNING("GlDefaultColorAttachment", "Attempted to bind default color attachment as shader resource at slot {}, this is not supported", slot)
    }

    void GlDefaultColorAttachment::clear() {
       glClearNamedFramebufferfv(0, GL_COLOR, 0, m_ClearColor);
    }

    void GlDefaultColorAttachment::onResize(u32 width, u32 height) {
        m_Width = width;
        m_Height = height;
    }

    UP<Image> GlDefaultColorAttachment::getData() {
        SR_LOG_WARNING("GlDefaultColorAttachment", "Attempted to get data from default color attachment which is not supported")
        return nullptr;
    }

    u64 GlDefaultColorAttachment::getIdentifier() const {
        return 0;
    }


}