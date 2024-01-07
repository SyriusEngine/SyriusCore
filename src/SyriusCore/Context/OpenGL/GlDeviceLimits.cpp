#include "GlDeviceLimits.hpp"

namespace Syrius{

    std::string GlDeviceLimits::getAPIVersion() {
        return getGlConstantStr(GL_VERSION);
    }

    std::string GlDeviceLimits::getDeviceName() {
        return getGlConstantStr(GL_RENDERER);
    }

    std::string GlDeviceLimits::getDeviceVendor() {
        return getGlConstantStr(GL_VENDOR);
    }

    std::string GlDeviceLimits::getShadingLanguageVersion() {
        return getGlConstantStr(GL_SHADING_LANGUAGE_VERSION);
    }

    uint32 GlDeviceLimits::getMaxFramebufferWidth() {
        return getGlConstantInt(GL_MAX_FRAMEBUFFER_WIDTH);
    }

    uint32 GlDeviceLimits::getMaxFramebufferHeight() {
        return getGlConstantInt(GL_MAX_FRAMEBUFFER_HEIGHT);
    }

    uint32 GlDeviceLimits::getMaxFramebufferColorAttachments() {
        return getGlConstantInt(GL_MAX_COLOR_ATTACHMENTS);
    }

    uint32 GlDeviceLimits::getMaxViewportWidth() {
        int32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return retVal[0];
    }

    uint32 GlDeviceLimits::getMaxViewportHeight() {
        int32 retVal[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, retVal);
        return retVal[1];
    }

    uint32 GlDeviceLimits::getMaxTextureSlots() {
        return getGlConstantInt(GL_MAX_TEXTURE_IMAGE_UNITS);
    }

    uint32 GlDeviceLimits::getMaxTextureSize() {
        return getGlConstantInt(GL_MAX_TEXTURE_SIZE);
    }

    uint32 GlDeviceLimits::getMaxSSSBOSlots() {
        return getGlConstantInt(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS);
    }

    uint32 GlDeviceLimits::getMaxConstantBufferSize() {
        return getGlConstantInt(GL_MAX_UNIFORM_BLOCK_SIZE);
    }

    uint32 GlDeviceLimits::getMaxConstantBufferSlots() {
        return getGlConstantInt(GL_MAX_UNIFORM_BUFFER_BINDINGS);
    }

    uint32 GlDeviceLimits::getMaxDepthBufferBits() {
        return getGlConstantInt(GL_DEPTH_BITS);
    }

    uint32 GlDeviceLimits::getMaxVertexAttributes() {
        return getGlConstantInt(GL_MAX_VERTEX_ATTRIBS);
    }

    uint32 GlDeviceLimits::getMaxVertices() {
        return getGlConstantInt(GL_MAX_ELEMENTS_VERTICES);
    }

    uint32 GlDeviceLimits::getMaxIndices() {
        return getGlConstantInt(GL_MAX_ELEMENTS_INDICES);
    }

    uint32 GlDeviceLimits::getMaxIndex() {
        return getGlConstantInt(GL_MAX_ELEMENT_INDEX);
    }

    bool GlDeviceLimits::isDoubleBuffered() {
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