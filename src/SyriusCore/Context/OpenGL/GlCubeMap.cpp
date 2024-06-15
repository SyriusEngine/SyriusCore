#include "GlCubeMap.hpp"
#include "GlTexture2D.hpp"
#include "GlColorAttachment.hpp"

namespace Syrius{

    GlCubeMap::GlCubeMap(const ResourceView<CubeMapLayout> &desc, const Resource<DeviceLimits> &deviceLimits) :
    CubeMap(desc, deviceLimits),
    m_TextureID(0),
    m_GlChannelFormat(getGlChannelFormat(desc->getFormat())),
    m_GlInternalFormat(getGlTextureFormatSized(desc->getFormat())),
    m_GlDataType(getGlTextureDataType(desc->getFormat())){

        if (!checkFormatSupported(m_GlInternalFormat)){
            SR_CORE_THROW("Internal format (%i) not supported, CubeMap object will be created but will not contain a"
                            "reference to a corresponding GPU object!", m_GlInternalFormat);
        }
        if (!checkFormatSupported(m_GlChannelFormat)){
            SR_CORE_THROW("Format (%i) not supported, CubeMap object will be created but will not contain a"
                            "reference to a corresponding GPU object!", m_GlChannelFormat);
        }

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        auto& faces = desc->getFaces();
        for (uint32 i = 0; i < 6; i++){
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

    void GlCubeMap::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[Texture2D]: Supplied slot (%i) is greater than the device number of texture slots (%i)", slot, m_DeviceLimits->getMaxTextureSlots());

        glBindTextureUnit(slot, m_TextureID);
    }

    void GlCubeMap::copyFrom(const ResourceView<Texture2D> &other, SR_CUBEMAP_FACE destinationFace) {
        SR_CORE_PRECONDITION(m_Width == other->getWidth(), "[GlTexture2D]: Width of the source texture (%i) does not match the width of the destination texture (%i)", other->getWidth(), m_Width);
        SR_CORE_PRECONDITION(m_Height == other->getHeight(), "[GlTexture2D]: Height of the source texture (%i) does not match the height of the destination texture (%i)", other->getHeight(), m_Height);
        SR_CORE_PRECONDITION(m_Format == other->getFormat(), "[GlTexture2D]: Format of the source texture (%i) does not match the format of the destination texture (%i)", other->getFormat(), m_Format);

//        glCopyImageSubData(other->getIdentifier(), GL_TEXTURE_2D, 0, 0, 0, 0,
//                           m_TextureID, GL_TEXTURE_CUBE_MAP, 0, 0, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace,
//                           m_Width, m_Height, 1);

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace, 0, 0, 0, 0, 0, m_Width, m_Height);
    }

    void GlCubeMap::copyFrom(const ResourceView<ColorAttachment> &other, SR_CUBEMAP_FACE destinationFace) {
        SR_CORE_PRECONDITION(m_Width == other->getWidth(), "[GlTexture2D]: Width of the source texture (%i) does not match the width of the destination texture (%i)", other->getWidth(), m_Width);
        SR_CORE_PRECONDITION(m_Height == other->getHeight(), "[GlTexture2D]: Height of the source texture (%i) does not match the height of the destination texture (%i)", other->getHeight(), m_Height);
        SR_CORE_PRECONDITION(m_Format == other->getFormat(), "[GlTexture2D]: Format of the source texture (%i) does not match the format of the destination texture (%i)", other->getFormat(), m_Format);

//        glCopyImageSubData(other->getIdentifier(), GL_TEXTURE_2D, 0, 0, 0, 0,
//                           m_TextureID, GL_TEXTURE_CUBE_MAP, 0, 0, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace,
//                           m_Width, m_Height, 1);

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace, 0, 0, 0, 0, 0, m_Width, m_Height);
    }

    Resource<Image> GlCubeMap::getData(SR_CUBEMAP_FACE destinationFace) {
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

        // TODO: I couldn't get the DSA method to work
//        glGetTextureSubImage(m_TextureID, 0, 0, 0, glFace, m_Width, m_Height, 1, m_GlChannelFormat, m_GlDataType, size, img->getData());

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + destinationFace, 0, m_GlChannelFormat, m_GlDataType, img->getData());

        return std::move(img);
    }

    uint64 GlCubeMap::getIdentifier() const {
        return m_TextureID;
    }

    bool GlCubeMap::checkFormatSupported(GLint toCheck) {
        GLint isSupported = GL_FALSE;
        glGetInternalformativ(GL_TEXTURE_CUBE_MAP, toCheck, GL_INTERNALFORMAT_SUPPORTED, 1, &isSupported);

        return isSupported == GL_TRUE;
    }

}