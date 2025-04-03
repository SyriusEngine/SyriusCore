#include "GlTexture2D.hpp"
#include "GlColorAttachment.hpp"

namespace Syrius{

    GlTexture2D::GlTexture2D(const Texture2DDesc& desc, const UP<DeviceLimits>& deviceLimits):
    Texture2D(desc, deviceLimits),
    m_TextureID(0),
    m_PixelUnpackBuffer(0),
    m_GlChannelFormat(getGlChannelFormat(desc.format)),
    m_GlInternalFormat(getGlTextureFormatSized(desc.format)),
    m_GlDataType(getGlTextureDataType(desc.format)){
        SR_PRECONDITION(deviceLimits->texture2DFormatSupported(desc.format), "GlTexture2D", "Texture format {} is not supported by the device", desc.format);

        createTexture(desc.data);
    }

    GlTexture2D::GlTexture2D(const Texture2DImageDesc &desc, const UP<DeviceLimits>& deviceLimits):
    Texture2D(desc, deviceLimits),
    m_TextureID(0),
    m_PixelUnpackBuffer(0),
    m_GlChannelFormat(getGlChannelFormat(desc.image->getFormat())),
    m_GlInternalFormat(getGlTextureFormatSized(desc.image->getFormat())),
    m_GlDataType(getGlTextureDataType(desc.image->getFormat())){
        SR_PRECONDITION(deviceLimits->texture2DFormatSupported(desc.image->getFormat()), "GlTexture2D", "Texture format {} is not supported by the device", desc.image->getFormat());

        createTexture(desc.image->getData());
    }

    GlTexture2D::~GlTexture2D() {
        if (m_PixelUnpackBuffer != 0){
            glDeleteBuffers(1, &m_PixelUnpackBuffer);
        }
        glDeleteTextures(1, &m_TextureID);
    }

    void GlTexture2D::bind() {
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void GlTexture2D::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[Texture2D]: Supplied slot {} is greater than the device number of texture slots {}}", slot, m_DeviceLimits->getMaxTextureSlots());

        glBindTextureUnit(slot, m_TextureID);
    }

    void GlTexture2D::setData(const void *data, u32 x, u32 y, u32 width, u32 height) {
        SR_PRECONDITION(data != nullptr, "[GlTexture2D]: Data is nullptr");
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlTexture2D]: Update on texture2D object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_PixelUnpackBuffer);
        SR_PRECONDITION(x + width <= m_Width, "[GlTexture2D]: Width {} exceeds the texture width {}", width, m_Width);
        SR_PRECONDITION(y + height <= m_Height, "[GLTexture2D]: Height {} exceeds the texture height {}", height, m_Height);
        SR_PRECONDITION(m_PixelUnpackBuffer != 0, "[GlTexture2D]: Pixel unpack buffer is not created for texture object {}", m_PixelUnpackBuffer);

        // use the pbo for more efficient data transfer
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PixelUnpackBuffer);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * getTextureChannelCount(m_Format), data, GL_DYNAMIC_DRAW);
        /*
         * DSA does currently not provide a function like glNamedReadPixelsToBuffer. so we have to bind the texture to the context
         */
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, m_GlChannelFormat, m_GlDataType, nullptr); // nullptr is used to indicate that the data is stored in the buffer bound to GL_PIXEL_UNPACK_BUFFER

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GlTexture2D::copyFrom(const ResourceView<Texture2D> &other) {
        SR_PRECONDITION(m_Width == other->getWidth(), "[GlTexture2D]: Width of the source texture {} does not match the width of the destination texture {}", other->getWidth(), m_Width);
        SR_PRECONDITION(m_Height == other->getHeight(), "[GlTexture2D]: Height of the source texture {} does not match the height of the destination texture {}", other->getHeight(), m_Height);
        SR_PRECONDITION(m_Format == other->getFormat(), "[GlTexture2D]: Format of the source texture {} does not match the format of the destination texture {}", other->getFormat(), m_Format);
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlTexture2D]: Copy on texture object {} requested, which has been created with SR_BUFFER_USAGE_STATIC flag!", m_PixelUnpackBuffer);

        glCopyImageSubData(other->getIdentifier(), GL_TEXTURE_2D, 0, 0, 0, 0, m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0, m_Width, m_Height, 1);

    }

    void GlTexture2D::copyFrom(const ResourceView<ColorAttachment> &other) {
        SR_PRECONDITION(m_Width == other->getWidth(), "[GlTexture2D]: Width of the source texture {} does not match the width of the destination texture {}", other->getWidth(), m_Width);
        SR_PRECONDITION(m_Height == other->getHeight(), "[GlTexture2D]: Height of the source texture {} does not match the height of the destination texture {}", other->getHeight(), m_Height);
        SR_PRECONDITION(m_Format == other->getFormat(), "[GlTexture2D]: Format of the source texture {} does not match the format of the destination texture {}", other->getFormat(), m_Format);
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlTexture2D]: Copy on texture object {} requested, which has been created with SR_BUFFER_USAGE_STATIC flag!", m_PixelUnpackBuffer);

        glCopyImageSubData(other->getIdentifier(), GL_TEXTURE_2D, 0, 0, 0, 0, m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0, m_Width, m_Height, 1);
    }

    UP<Image> GlTexture2D::getData() {
        SR_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlTexture2D]: Update on texture2D object {} requested, which was created with SR_BUFFER_USAGE_STATIC flag!", m_PixelUnpackBuffer);

        auto channelCount = getTextureChannelCount(m_Format);
        ImageUI8Desc imgDesc;
        imgDesc.width = m_Width;
        imgDesc.height = m_Height;
        switch (channelCount) {
            case 1: imgDesc.format = SR_TEXTURE_R_UI8; break;
            case 2: imgDesc.format = SR_TEXTURE_RG_UI8; break;
            case 3: imgDesc.format = SR_TEXTURE_RGB_UI8; break;
            case 4: imgDesc.format = SR_TEXTURE_RGBA_UI8; break;
            default: SR_LOG_THROW("GlTexture2D", "Invalid channel count {} for texture object {}", channelCount, m_PixelUnpackBuffer);
        }
        auto img = createImage(imgDesc);
        auto size = m_Width * m_Height * channelCount;
        glGetTextureImage(m_TextureID, 0, m_GlChannelFormat, m_GlDataType, size, img->getData());
        return std::move(img);
    }

    u64 GlTexture2D::getIdentifier() const {
        return m_TextureID;
    }

    void GlTexture2D::createTexture(const void* data) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, m_GlInternalFormat, m_Width, m_Height);
        if (data != nullptr){
            glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, m_GlChannelFormat, m_GlDataType, data);
        }
        glGenerateTextureMipmap(m_TextureID);

        if (m_Usage != SR_BUFFER_USAGE_STATIC){
            glCreateBuffers(1, &m_PixelUnpackBuffer);
        }
    }


}