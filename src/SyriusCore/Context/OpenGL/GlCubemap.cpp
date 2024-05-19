#include "GlCubemap.hpp"

namespace Syrius{

    GlCubemap::GlCubemap(const CubemapDesc &desc, const Resource<DeviceLimits>& deviceLimits) :
            CubeMap(desc, deviceLimits),
            m_TextureID(0) {
        const void* faces[6] = {
                desc.faces[0],
                desc.faces[1],
                desc.faces[2],
                desc.faces[3],
                desc.faces[4],
                desc.faces[5]
        };
        createCubemap(faces);
    }

    GlCubemap::GlCubemap(const CubemapImageDesc &desc, const Resource<DeviceLimits>& deviceLimits) :
            CubeMap(desc, deviceLimits),
            m_TextureID(0) {
        const void* faces[6] = {
                desc.faces[0]->getData(),
                desc.faces[1]->getData(),
                desc.faces[2]->getData(),
                desc.faces[3]->getData(),
                desc.faces[4]->getData(),
                desc.faces[5]->getData()
        };

        createCubemap(faces);
    }

    GlCubemap::~GlCubemap() {
        glDeleteTextures(1, &m_TextureID);
    }

    void GlCubemap::bind() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    }

    void GlCubemap::bindShaderResource(uint32 slot) {
        SR_CORE_PRECONDITION(slot < m_DeviceLimits->getMaxTextureSlots(), "[Texture2D]: Supplied slot (%i) is greater than the device number of texture slots (%i)", slot, m_DeviceLimits->getMaxTextureSlots());

        glBindTextureUnit(slot, m_TextureID);
    }

    uint64 GlCubemap::getIdentifier() const {
        return m_TextureID;
    }

    bool GlCubemap::checkFormatSupported(GLint toCheck) {
        GLint isSupported = GL_FALSE;
        glGetInternalformativ(GL_TEXTURE_CUBE_MAP, toCheck, GL_INTERNALFORMAT_SUPPORTED, 1, &isSupported);

        return isSupported == GL_TRUE;
    }

    void GlCubemap::determineFormats() {
        m_GlDataType = getGlTextureDataType(m_Format);
        m_GlInternalFormat = getGlTextureFormat(m_Format);
        m_GlFormat = getGlChannelType(getTextureChannelFormat(m_Format));

        SR_CORE_POSTCONDITION(checkFormatSupported(m_GlInternalFormat), "Internal format (%i) not supported", m_GlInternalFormat);
        SR_CORE_POSTCONDITION(checkFormatSupported(m_GlFormat), "Format (%i) not supported", m_GlFormat);
    }

    void GlCubemap::createCubemap(const void* faces[6]) {
        determineFormats();
        /**
         * Note: On both Nvidia and Intel GPUs, the DSA implementation works perfectly fine, but on AMD GPUs, it doesn't work.
         * The first texture will be uploaded correctly, but the rest will be black.
         */
//        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_TextureID);
//        glTextureStorage2D(m_TextureID, 1, m_GlInternalFormat, m_Width, m_Height);
//        for (uint32 i = 0; i < 6; i++){
//            glTextureSubImage3D(m_TextureID, 0, 0, 0, i, m_Width, m_Height, 1, m_GlFormat, m_GlDataType, faces[i].data);
//        }

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        for (uint32 i = 0; i < 6; i++){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_GlInternalFormat, m_Width, m_Height, 0, m_GlFormat, m_GlDataType, faces[i]);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



    }
}