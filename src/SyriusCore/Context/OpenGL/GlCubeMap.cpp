#include "GlCubeMap.hpp"
#include "GlTexture2D.hpp"
#include "GlColorAttachment.hpp"

namespace Syrius{

    GlCubeMap::GlCubeMap(const ResourceView<CubeMapLayout> &desc, const UP<DeviceLimits> &deviceLimits) :
    CubeMap(desc, deviceLimits),
    m_TextureID(0),
    m_GlChannelFormat(getGlChannelFormat(desc->getFormat())),
    m_GlInternalFormat(getGlTextureFormatSized(desc->getFormat())),
    m_GlDataType(getGlTextureDataType(desc->getFormat())){
        SR_PRECONDITION(checkFormatSupported(m_GlInternalFormat), "GlCubemap", "Internal format {} not supported", m_GlInternalFormat);
        SR_PRECONDITION(checkFormatSupported(m_GlChannelFormat), "GlCubemap", "Format {} not supported", m_GlChannelFormat);

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        auto& faces = desc->getFaces();
        for (u32 i = 0; i < 6; i++){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_GlInternalFormat, m_Width, m_Height, 0, m_GlChannelFormat, m_GlDataType, faces[i]->getData());
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }


    GlCubeMap::~GlCubeMap() {
        glDeleteTextures(1, &m_TextureID);
    }

    void GlCubeMap::bind() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    }

    void GlCubeMap::bindShaderResource(u32 slot) {
        SR_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[Texture2D]: Supplied slot {} is greater than the device number of texture slots {}", slot, m_DeviceLimits->getMaxTextureSlots());

        glBindTextureUnit(slot, m_TextureID);
    }

    void GlCubeMap::copyFrom(const ResourceView<Texture2D> &other, SR_CUBEMAP_FACE destinationFace) {
        SR_PRECONDITION(m_Width == other->getWidth(), "[GlTexture2D]: Width of the source texture {} does not match the width of the destination texture {}", other->getWidth(), m_Width);
        SR_PRECONDITION(m_Height == other->getHeight(), "[GlTexture2D]: Height of the source texture {} does not match the height of the destination texture {}", other->getHeight(), m_Height);
        SR_PRECONDITION(m_Format == other->getFormat(), "[GlTexture2D]: Format of the source texture {} does not match the format of the destination texture {}", other->getFormat(), m_Format);

//        glCopyImageSubData(other->getIdentifier(), GL_TEXTURE_2D, 0, 0, 0, 0,
//                           m_TextureID, GL_TEXTURE_CUBE_MAP, 0, 0, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace,
//                           m_Width, m_Height, 1);

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace, 0, 0, 0, 0, 0, m_Width, m_Height);
    }

    void GlCubeMap::copyFrom(const ResourceView<ColorAttachment> &other, SR_CUBEMAP_FACE destinationFace) {
        SR_PRECONDITION(m_Width == other->getWidth(), "[GlTexture2D]: Width of the source texture {} does not match the width of the destination texture {}", other->getWidth(), m_Width);
        SR_PRECONDITION(m_Height == other->getHeight(), "[GlTexture2D]: Height of the source texture {} does not match the height of the destination texture {}", other->getHeight(), m_Height);
        SR_PRECONDITION(m_Format == other->getFormat(), "[GlTexture2D]: Format of the source texture {} does not match the format of the destination texture {}", other->getFormat(), m_Format);

//        glCopyImageSubData(other->getIdentifier(), GL_TEXTURE_2D, 0, 0, 0, 0,
//                           m_TextureID, GL_TEXTURE_CUBE_MAP, 0, 0, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace,
//                           m_Width, m_Height, 1);

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace, 0, 0, 0, 0, 0, m_Width, m_Height);
    }

    UP<Image> GlCubeMap::getData(SR_CUBEMAP_FACE destinationFace) {
        auto channelCount = getTextureChannelCount(m_Format);
        ImageUI8Desc imgDesc;
        imgDesc.width = m_Width;
        imgDesc.height = m_Height;
        switch (channelCount) {
            case 1: imgDesc.format = SR_TEXTURE_R_UI8; break;
            case 2: imgDesc.format = SR_TEXTURE_RG_UI8; break;
            case 3: imgDesc.format = SR_TEXTURE_RGB_UI8; break;
            case 4: imgDesc.format = SR_TEXTURE_RGBA_UI8; break;
            default: SR_LOG_THROW("GlTexture2D", "Invalid channel count {} for texture object {}", channelCount, m_TextureID);
        }
        auto img = createImage(imgDesc);

        // TODO: I couldn't get the DSA method to work
//        glGetTextureSubImage(m_TextureID, 0, 0, 0, glFace, m_Width, m_Height, 1, m_GlChannelFormat, m_GlDataType, size, img->getData());

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace, 0, m_GlChannelFormat, m_GlDataType, img->getData());

        return std::move(img);
    }

    u64 GlCubeMap::getIdentifier() const {
        return m_TextureID;
    }

    bool GlCubeMap::checkFormatSupported(GLint toCheck) {
        GLint isSupported = GL_FALSE;
        glGetInternalformativ(GL_TEXTURE_CUBE_MAP, toCheck, GL_INTERNALFORMAT_SUPPORTED, 1, &isSupported);

        return isSupported == GL_TRUE;
    }

}