#include "GlDeviceLimits.hpp"

namespace Syrius{

    std::string GlDeviceLimits::getAPIVersion() const {
        return getGlConstantStr(GL_VERSION);
    }

    std::string GlDeviceLimits::getDeviceName() const {
        return getGlConstantStr(GL_RENDERER);
    }

    std::string GlDeviceLimits::getDeviceVendor() const {
        return getGlConstantStr(GL_VENDOR);
    }

    std::string GlDeviceLimits::getShadingLanguageVersion() const {
        return getGlConstantStr(GL_SHADING_LANGUAGE_VERSION);
    }

    u32 GlDeviceLimits::getMaxFramebufferWidth() const {
        return getGlConstantInt(GL_MAX_FRAMEBUFFER_WIDTH);
    }

    u32 GlDeviceLimits::getMaxFramebufferHeight() const {
        return getGlConstantInt(GL_MAX_FRAMEBUFFER_HEIGHT);
    }

    u32 GlDeviceLimits::getMaxFramebufferColorAttachments() const {
        return getGlConstantInt(GL_MAX_COLOR_ATTACHMENTS);
    }

    u32 GlDeviceLimits::getMaxViewportWidth() const {
        i32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return retVal[0];
    }

    u32 GlDeviceLimits::getMaxViewportHeight() const {
        i32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return retVal[1];
    }

    u32 GlDeviceLimits::getMaxTextureSlots() const {
        return getGlConstantInt(GL_MAX_TEXTURE_IMAGE_UNITS);
    }

    u32 GlDeviceLimits::getMaxTextureSize() const {
        return getGlConstantInt(GL_MAX_TEXTURE_SIZE);
    }

    u32 GlDeviceLimits::getMaxSSSBOSlots() const {
        return getGlConstantInt(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS);
    }

    u32 GlDeviceLimits::getMaxConstantBufferSize() const {
        return getGlConstantInt(GL_MAX_UNIFORM_BLOCK_SIZE);
    }

    u32 GlDeviceLimits::getMaxConstantBufferSlots() const {
        return getGlConstantInt(GL_MAX_UNIFORM_BUFFER_BINDINGS);
    }

    u32 GlDeviceLimits::getMaxDepthBufferBits() const {
        return getGlConstantInt(GL_DEPTH_BITS);
    }

    u32 GlDeviceLimits::getMaxVertexAttributes() const {
        return getGlConstantInt(GL_MAX_VERTEX_ATTRIBS);
    }

    u64 GlDeviceLimits::getMaxVertexCount() const {
        return getGlConstantInt(GL_MAX_ELEMENTS_VERTICES);
    }

    u64 GlDeviceLimits::getMaxIndexCount() const {
        return getGlConstantInt(GL_MAX_ELEMENTS_INDICES);
    }

    u64 GlDeviceLimits::getMaxIndex() const {
        return getGlConstantInt(GL_MAX_ELEMENT_INDEX);
    }

    bool GlDeviceLimits::isDoubleBuffered() const {
        return getGlConstantInt(GL_DOUBLEBUFFER) == 1;
    }

    std::string GlDeviceLimits::getGlConstantStr(GLenum constant) {
        const unsigned char* version = glGetString(constant);
        char* temp = (char*) version;
        std::string retVal = temp;
        return retVal;
    }

    u32 GlDeviceLimits::getGlConstantInt(GLenum constant) {
        i32 retVal = 0;
        glGetIntegerv(constant, &retVal);
        return retVal;
    }

    bool GlDeviceLimits::texture2DFormatSupported(SR_TEXTURE_FORMAT format) {
        if (m_SupportedTexture2DFormats.find(format) != m_SupportedTexture2DFormats.end()){
            return m_SupportedTexture2DFormats.at(format);
        }
        GLint supportedFormat = GL_FALSE;
        auto glFormat = getGlTextureFormat(format);
        glGetInternalformativ(GL_TEXTURE_2D, glFormat, GL_INTERNALFORMAT_SUPPORTED, 1, &supportedFormat);
        m_SupportedTexture2DFormats[format] = supportedFormat == GL_TRUE;
        return supportedFormat == GL_TRUE;
    }
}