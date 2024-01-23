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

    uint32 GlDeviceLimits::getMaxFramebufferWidth() const {
        return getGlConstantInt(GL_MAX_FRAMEBUFFER_WIDTH);
    }

    uint32 GlDeviceLimits::getMaxFramebufferHeight() const {
        return getGlConstantInt(GL_MAX_FRAMEBUFFER_HEIGHT);
    }

    uint32 GlDeviceLimits::getMaxFramebufferColorAttachments() const {
        return getGlConstantInt(GL_MAX_COLOR_ATTACHMENTS);
    }

    uint32 GlDeviceLimits::getMaxViewportWidth() const {
        int32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return retVal[0];
    }

    uint32 GlDeviceLimits::getMaxViewportHeight() const {
        int32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return retVal[1];
    }

    uint32 GlDeviceLimits::getMaxTextureSlots() const {
        return getGlConstantInt(GL_MAX_TEXTURE_IMAGE_UNITS);
    }

    uint32 GlDeviceLimits::getMaxTextureSize() const {
        return getGlConstantInt(GL_MAX_TEXTURE_SIZE);
    }

    uint32 GlDeviceLimits::getMaxShaderStorageBufferSlots() const {
        return getGlConstantInt(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS);
    }

    uint32 GlDeviceLimits::getMaxConstantBufferSize() const {
        return getGlConstantInt(GL_MAX_UNIFORM_BLOCK_SIZE);
    }

    uint32 GlDeviceLimits::getMaxConstantBufferSlots() const {
        return getGlConstantInt(GL_MAX_UNIFORM_BUFFER_BINDINGS);
    }

    uint32 GlDeviceLimits::getMaxDepthBufferBits() const {
        return getGlConstantInt(GL_DEPTH_BITS);
    }

    uint32 GlDeviceLimits::getMaxVertexAttributes() const {
        return getGlConstantInt(GL_MAX_VERTEX_ATTRIBS);
    }

    uint64 GlDeviceLimits::getMaxVertexCount() const {
        return getGlConstantInt(GL_MAX_ELEMENTS_VERTICES);
    }

    uint64 GlDeviceLimits::getMaxIndexCount() const {
        return getGlConstantInt(GL_MAX_ELEMENTS_INDICES);
    }

    uint64 GlDeviceLimits::getMaxIndex() const {
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

    uint32 GlDeviceLimits::getGlConstantInt(GLenum constant) {
        int32 retVal = 0;
        glGetIntegerv(constant, &retVal);
        return retVal;
    }
}