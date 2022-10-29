#include "GlContext.hpp"

namespace Syrius{

    uint64 GlContext::m_ActiveContextID = 0;

    GlContext::GlContext(const ContextDesc& desc)
    : Context(desc),
    m_ID(generateID()){
        m_ActiveContextID = m_ID;
    }

    GlContext::~GlContext() {
        delete m_DefaultFrameBuffer;

        CoreCommand::terminateGlad();
    }

    void GlContext::initGl(const FrameBufferDesc& desc) {
        CoreCommand::initGlad();

        m_DefaultFrameBuffer = new GlDefaultFramebuffer(desc);
    }

    std::string GlContext::getAPIVersion() {
        ensureCurrentContext();
        const unsigned char* version = glGetString(GL_VERSION);
        char* temp = (char*) version;
        std::string retVal = temp;
        return retVal;
    }

    std::string GlContext::getDeviceName() {
        ensureCurrentContext();
        const unsigned char* version = glGetString(GL_RENDERER);
        char* temp = (char*) version;
        std::string retVal = temp;
        return retVal;
    }

    std::string GlContext::getShadingLanguageVersion() {
        ensureCurrentContext();
        const unsigned char* version = glGetString(GL_SHADING_LANGUAGE_VERSION);
        char* temp = (char*) version;
        std::string retVal = temp;
        return retVal;
    }

    int32 GlContext::getMaxFramebufferWidth() {
        ensureCurrentContext();
        int32 maxFramebufferWidth;
        glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFramebufferWidth);
        return maxFramebufferWidth;
    }

    int32 GlContext::getMaxFramebufferHeight() {
        ensureCurrentContext();
        int32 maxFramebufferHeight;
        glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFramebufferHeight);
        return maxFramebufferHeight;
    }

    int32 GlContext::getMaxFramebufferTextureAttachments() {
        ensureCurrentContext();
        int32 maxAttach;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);
        return maxAttach;
    }

    int32 GlContext::getMaxTextureSlots() {
        ensureCurrentContext();
        int32 maxTextureSlotCount;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlotCount);
        return maxTextureSlotCount;
    }

    int32 GlContext::getMaxTexture2DSize() {
        ensureCurrentContext();
        int32 maxTextureSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        return maxTextureSize;
    }

    int32 GlContext::getMaxConstantBufferSize() {
        ensureCurrentContext();
        int32 maxCbufferSize;
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxCbufferSize);
        return maxCbufferSize;
    }

    int32 GlContext::getMaxDepthBufferBits() {
        ensureCurrentContext();
        int32 depthBufferBits;
        glGetIntegerv(GL_DEPTH_BITS, &depthBufferBits);
        return depthBufferBits;
    }

    ShaderModule *GlContext::createShaderModule(const ShaderModuleDesc &desc) {
        ensureCurrentContext();
        auto ptr = new GlShaderModule(desc);
        m_ShaderModules.push_back(ptr);
        return ptr;
    }

    Shader *GlContext::createShader(const ShaderDesc& desc) {
        ensureCurrentContext();
        auto ptr = new GlShader(desc);
        m_Shaders.push_back(ptr);
        return ptr;
    }

    VertexBuffer *GlContext::createVertexBuffer(const VertexBufferDesc &desc) {
        ensureCurrentContext();
        auto ptr = new GlVertexBuffer(desc);
        m_VertexBuffers.push_back(ptr);
        return ptr;
    }

    IndexBuffer *GlContext::createIndexBuffer(const IndexBufferDesc &desc) {
        ensureCurrentContext();
        auto ptr = new GlIndexBuffer(desc);
        m_IndexBuffers.push_back(ptr);
        return ptr;
    }

    VertexArray *GlContext::createVertexArray(const VertexArrayDesc &desc) {
        ensureCurrentContext();
        VertexArray* ptr;
        if (desc.m_IndexBuffer != nullptr) {
            ptr = new GlVertexArrayIndexed(desc);
        }
        else {
            ptr = new GlVertexArray(desc);
        }
        m_VertexArrays.push_back(ptr);
        return ptr;
    }

    ConstantBuffer *GlContext::createConstantBuffer(const ConstantBufferDesc &desc) {
        SR_CORE_PRECONDITION(desc.m_Size <= getMaxConstantBufferSize(), "Constant buffer size is too large");

        ensureCurrentContext();
        auto ptr = new GlConstantBuffer(desc);
        m_ConstantBuffers.push_back(ptr);
        return ptr;
    }

    FrameBuffer *GlContext::createFrameBuffer(const FrameBufferDesc &desc) {
        SR_CORE_PRECONDITION(!desc.m_ColorAttachments.empty(), "Framebuffer must have at least one color attachment");
        SR_CORE_PRECONDITION(desc.m_Width <= getMaxFramebufferWidth(), "Framebuffer width is too large");
        SR_CORE_PRECONDITION(desc.m_Height <= getMaxFramebufferHeight(), "Framebuffer height is too large");

        ensureCurrentContext();
        auto ptr = new GlFrameBuffer(desc);
        m_FrameBuffers.push_back(ptr);
        return ptr;
    }

    Texture2D *GlContext::createTexture2D(const Texture2DDesc& desc) {
        SR_CORE_PRECONDITION(desc.m_Image != nullptr, "Texture image is null");
        SR_CORE_PRECONDITION(desc.m_Sampler2D != nullptr, "Texture sampler is null");
        SR_CORE_PRECONDITION(desc.m_Image->getWidth() <= getMaxTexture2DSize(), "Texture width is too large");
        SR_CORE_PRECONDITION(desc.m_Image->getHeight() <= getMaxTexture2DSize(), "Texture height is too large");

        ensureCurrentContext();

        auto ptr = new GlTexture2D(desc);
        m_Textures2D.push_back(ptr);
        return ptr;
    }

    Sampler *GlContext::createSampler2D(const Sampler2DDesc &desc) {
        auto ptr = new GlSampler(desc);
        m_Samplers2D.push_back(ptr);
        return ptr;
    }

}

