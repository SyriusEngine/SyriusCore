#include "GlTexture2D.hpp"

namespace Syrius{

    GlTexture2D::GlTexture2D(const Texture2DDesc& desc, const Resource<DeviceLimits>& deviceLimits):
    Texture2D(desc, deviceLimits),
    m_TextureID(0){
        createTexture(desc.data);
    }

    GlTexture2D::GlTexture2D(const Texture2DImageDesc &desc, const Resource<DeviceLimits>& deviceLimits):
    Texture2D(desc, deviceLimits),
    m_TextureID(0){
        createTexture(desc.image->getData());
    }

    GlTexture2D::~GlTexture2D() {
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
        SR_CORE_PRECONDITION(m_Usage == SR_BUFFER_USAGE_DYNAMIC, "[Texture2D]: Update on texture object (%p) requested, which has not been created with SR_BUFFER_USAGE_DYNAMIC flag!", this);
        SR_CORE_PRECONDITION(x + width <= m_Width, "[Texture2D]: Width (%i) exceeds the texture width (%i)", width, m_Width);
        SR_CORE_PRECONDITION(y + height <= m_Height, "[Texture2D]: Height (%i) exceeds the texture height (%i)", height, m_Height);

        glTextureSubImage2D(m_TextureID, 0, x, y, width, height, m_GlFormat, m_GlDataType, data);
    }

    Resource<Image> GlTexture2D::getData() {
        auto channelCount = getTextureChannelCount(m_Format);
        auto size = m_Width * m_Height * channelCount;
        auto data = Resource<uint8>(new uint8[size]);
        glGetTextureImage(m_TextureID, 0, m_GlFormat, m_GlDataType, size, data.get());
        ImageUI8Desc imgDesc;
        imgDesc.width = m_Width;
        imgDesc.height = m_Height;
        imgDesc.format = m_Format;
        imgDesc.data = data.get();
        return createImageUI8(imgDesc);
    }

    uint64 GlTexture2D::getIdentifier() const {
        return m_TextureID;
    }

    void GlTexture2D::setGlFormats() {
        m_GlDataType = getGlTextureDataType(m_Format);
        auto channelFormat = getTextureChannelFormat(m_Format);
        m_GlFormat = getGlChannelType(channelFormat);
        m_GlInternalFormat = getGlTextureFormat(m_Format);
    }

    void GlTexture2D::createTexture(const void* data) {
        setGlFormats();
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glTextureStorage2D(m_TextureID, 1, m_GlInternalFormat, m_Width, m_Height);
        if (data != nullptr){
            glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, m_GlFormat, m_GlDataType, data);
        }
    }


}