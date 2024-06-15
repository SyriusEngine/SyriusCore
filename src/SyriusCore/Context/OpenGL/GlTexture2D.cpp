#include "GlTexture2D.hpp"
#include "GlColorAttachment.hpp"

namespace Syrius{

    GlTexture2D::GlTexture2D(const Texture2DDesc& desc, const Resource<DeviceLimits>& deviceLimits):
    Texture2D(desc, deviceLimits),
    m_TextureID(0),
    m_PixelUnpackBuffer(0),
    m_GlChannelFormat(getGlChannelFormat(desc.format)),
    m_GlInternalFormat(getGlTextureFormatSized(desc.format)),
    m_GlDataType(getGlTextureDataType(desc.format)){
        if (!deviceLimits->texture2DFormatSupported(desc.format)){
            SR_CORE_THROW("[GlTexture2D]: Texture format (%i) is not supported by the device", desc.format);
        }
        createTexture(desc.data);
    }

    GlTexture2D::GlTexture2D(const Texture2DImageDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    Texture2D(desc, deviceLimits),
    m_TextureID(0),
    m_PixelUnpackBuffer(0),
    m_GlChannelFormat(getGlChannelFormat(desc.image->getFormat())),
    m_GlInternalFormat(getGlTextureFormatSized(desc.image->getFormat())),
    m_GlDataType(getGlTextureDataType(desc.image->getFormat())){
        if (!deviceLimits->texture2DFormatSupported(desc.image->getFormat())){
            SR_CORE_THROW("[GlTexture2D]: Texture format (%i) is not supported by the device", desc.image->getFormat());
        }
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

    void GlTexture2D::bindShaderResource(uint32_t slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[Texture2D]: Supplied slot (%i) is greater than the device number of texture slots (%i)", slot, m_DeviceLimits->getMaxTextureSlots());

        glBindTextureUnit(slot, m_TextureID);
    }

    void GlTexture2D::setData(const void *data, uint32 x, uint32 y, uint32 width, uint32 height) {
        SR_CORE_PRECONDITION(data != nullptr, "[GlTexture2D]: Data is nullptr (%p)", data);
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlTexture2D]: Update on texture2D object (%p) requested, which was created with SR_BUFFER_USAGE_STATIC flag!", this);
        SR_CORE_PRECONDITION(x + width <= m_Width, "[GlTexture2D]: Width (%i) exceeds the texture width (%i)", width, m_Width);
        SR_CORE_PRECONDITION(y + height <= m_Height, "[GLTexture2D]: Height (%i) exceeds the texture height (%i)", height, m_Height);
        SR_CORE_PRECONDITION(m_PixelUnpackBuffer != 0, "[GlTexture2D]: Pixel unpack buffer is not created for texture object (%p)", this);

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
        SR_CORE_PRECONDITION(m_Width == other->getWidth(), "[GlTexture2D]: Width of the source texture (%i) does not match the width of the destination texture (%i)", other->getWidth(), m_Width);
        SR_CORE_PRECONDITION(m_Height == other->getHeight(), "[GlTexture2D]: Height of the source texture (%i) does not match the height of the destination texture (%i)", other->getHeight(), m_Height);
        SR_CORE_PRECONDITION(m_Format == other->getFormat(), "[GlTexture2D]: Format of the source texture (%i) does not match the format of the destination texture (%i)", other->getFormat(), m_Format);
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlTexture2D]: Copy on texture object (%p) requested, which has been created with SR_BUFFER_USAGE_STATIC flag!", this);

        glCopyImageSubData(other->getIdentifier(), GL_TEXTURE_2D, 0, 0, 0, 0, m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0, m_Width, m_Height, 1);

    }

    void GlTexture2D::copyFrom(const ResourceView<ColorAttachment> &other) {
        SR_CORE_PRECONDITION(m_Width == other->getWidth(), "[GlTexture2D]: Width of the source texture (%i) does not match the width of the destination texture (%i)", other->getWidth(), m_Width);
        SR_CORE_PRECONDITION(m_Height == other->getHeight(), "[GlTexture2D]: Height of the source texture (%i) does not match the height of the destination texture (%i)", other->getHeight(), m_Height);
        SR_CORE_PRECONDITION(m_Format == other->getFormat(), "[GlTexture2D]: Format of the source texture (%i) does not match the format of the destination texture (%i)", other->getFormat(), m_Format);
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlTexture2D]: Copy on texture object (%p) requested, which has been created with SR_BUFFER_USAGE_STATIC flag!", this);

        glCopyImageSubData(other->getIdentifier(), GL_TEXTURE_2D, 0, 0, 0, 0, m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0, m_Width, m_Height, 1);
    }

    Resource<Image> GlTexture2D::getData() {
        SR_CORE_PRECONDITION(m_Usage != SR_BUFFER_USAGE_STATIC, "[GlTexture2D]: Update on texture2D object (%p) requested, which was created with SR_BUFFER_USAGE_STATIC flag!", this);

        auto channelCount = getTextureChannelCount(m_Format);
        ImageUI8Desc imgDesc;
        imgDesc.width = m_Width;
        imgDesc.height = m_Height;
        switch (channelCount) {
            case 1: imgDesc.format = SR_TEXTURE_R_UI8; break;
            case 2: imgDesc.format = SR_TEXTURE_RG_UI8; break;
            case 3: imgDesc.format = SR_TEXTURE_RGB_UI8; break;
            case 4: imgDesc.format = SR_TEXTURE_RGBA_UI8; break;
            default: SR_CORE_THROW("[GlTexture2D]: Invalid channel count (%i) for texture object (%p)", channelCount, this);
        }
        auto img = createImage(imgDesc);
        auto size = m_Width * m_Height * channelCount;
        glGetTextureImage(m_TextureID, 0, m_GlChannelFormat, m_GlDataType, size, img->getData());
        return std::move(img);
    }

    uint64 GlTexture2D::getIdentifier() const {
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