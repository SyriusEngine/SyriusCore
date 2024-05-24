#include "GlCubeMap.hpp"

namespace Syrius{

    GlCubeMap::GlCubeMap(const ResourceView<CubeMapLayout> &desc, const Resource<DeviceLimits> &deviceLimits) :
    CubeMap(desc, deviceLimits),
    m_TextureID(0){
        m_GlDataType = getGlTextureDataType(m_Format);
        m_GlInternalFormat = getGlTextureFormat(m_Format);
        m_GlFormat = getGlChannelType(getTextureChannelFormat(m_Format));

        if (!checkFormatSupported(m_GlInternalFormat)){
            SR_CORE_THROW("Internal format (%i) not supported, CubeMap object will be created but will not contain a"
                            "reference to a corresponding GPU object!", m_GlInternalFormat);
        }
        if (!checkFormatSupported(m_GlFormat)){
            SR_CORE_THROW("Format (%i) not supported, CubeMap object will be created but will not contain a"
                            "reference to a corresponding GPU object!", m_GlFormat);
        }

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
        auto& faces = desc->getFaces();
        for (uint32 i = 0; i < 6; i++){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_GlInternalFormat, m_Width, m_Height, 0, m_GlFormat, m_GlDataType, faces[i]->getData());
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

    uint64 GlCubeMap::getIdentifier() const {
        return m_TextureID;
    }

    bool GlCubeMap::checkFormatSupported(GLint toCheck) {
        GLint isSupported = GL_FALSE;
        glGetInternalformativ(GL_TEXTURE_CUBE_MAP, toCheck, GL_INTERNALFORMAT_SUPPORTED, 1, &isSupported);

        return isSupported == GL_TRUE;
    }
}