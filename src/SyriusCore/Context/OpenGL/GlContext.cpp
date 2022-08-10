#include "GlContext.hpp"

namespace Syrius{

    GlContext::GlContext()
    : Context(SR_API_OPENGL){

    }

    GlContext::~GlContext() {
        CoreCommand::terminateGlad();
    }

    void GlContext::initGl() {
        CoreCommand::initGlad();
    }

    std::string GlContext::getAPIVersion() {
        const unsigned char* version = glGetString(GL_VERSION);
        char* temp = (char*) version;
        std::string retVal = temp;
        return retVal;
    }

    std::string GlContext::getDeviceName() {
        const unsigned char* version = glGetString(GL_RENDERER);
        char* temp = (char*) version;
        std::string retVal = temp;
        return retVal;
    }

    std::string GlContext::getShadingLanguageVersion() {
        const unsigned char* version = glGetString(GL_SHADING_LANGUAGE_VERSION);
        char* temp = (char*) version;
        std::string retVal = temp;
        return retVal;
    }

    int32 GlContext::getMaxFramebufferWidth() {
        int32 maxFramebufferWidth;
        glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFramebufferWidth);
        return maxFramebufferWidth;
    }

    int32 GlContext::getMaxFramebufferHeight() {
        int32 maxFramebufferHeight;
        glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFramebufferHeight);
        return maxFramebufferHeight;
    }

    int32 GlContext::getMaxFramebufferTextureAttachments() {
        int32 maxAttach;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);
        return maxAttach;
    }

    int32 GlContext::getMaxTextureSlots() {
        int32 maxTextureSlotCount;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlotCount);
        return maxTextureSlotCount;
    }

    int32 GlContext::getMaxTexture2DSize() {
        int32 maxTextureSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        return maxTextureSize;
    }

    int32 GlContext::getMaxConstantBufferSize() {
        int32 maxCbufferSize;
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxCbufferSize);
        return maxCbufferSize;
    }

    int32 GlContext::getMaxDepthBufferBits() {
        int32 depthBufferBits;
        glGetIntegerv(GL_DEPTH_BITS, &depthBufferBits);
        return depthBufferBits;
    }

    ShaderModule *GlContext::createShaderModule(const ShaderModuleDesc &desc) {
        return new GlShaderModule(desc);
    }

    Shader *GlContext::createShader() {
        return new GlShader();
    }

}

