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

    std::string GlContext::getDeviceVendor() {
        const unsigned char* version = glGetString(GL_VENDOR);
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
        SR_CORE_PRECONDITION(desc.m_VertexShader != nullptr, "Vertex shader is null");
        SR_CORE_PRECONDITION(desc.m_FragmentShader != nullptr, "Fragment shader is null");

        ensureCurrentContext();
        auto ptr = new GlShader(desc);
        m_Shaders.push_back(ptr);

        SR_CORE_POSTCONDITION(ptr != nullptr, "Shader creation failed");
        return ptr;
    }

    VertexBuffer *GlContext::createVertexBuffer(const VertexBufferDesc &desc) {
        SR_CORE_PRECONDITION(desc.m_Count > 0, "Vertex count is 0");
        SR_CORE_PRECONDITION(desc.m_Layout != nullptr, "Vertex layout is null");
        SR_CORE_PRECONDITION(desc.m_Layout->getAttributeCount() > 0, "Vertex layout has no attributes");

        ensureCurrentContext();
        auto ptr = new GlVertexBuffer(desc);
        m_VertexBuffers.push_back(ptr);

        SR_CORE_POSTCONDITION(ptr != nullptr, "Vertex buffer is null");

        return ptr;
    }

    IndexBuffer *GlContext::createIndexBuffer(const IndexBufferDesc &desc) {
        SR_CORE_PRECONDITION(desc.m_Count > 0, "Index count is 0");

        ensureCurrentContext();
        auto ptr = new GlIndexBuffer(desc);
        m_IndexBuffers.push_back(ptr);

        SR_CORE_POSTCONDITION(ptr != nullptr, "Index buffer is null");

        return ptr;
    }

    VertexArray *GlContext::createVertexArray(const VertexArrayDesc &desc) {
        SR_CORE_PRECONDITION(desc.m_VertexBuffer != nullptr, "Vertex buffer is null");

        ensureCurrentContext();
        VertexArray* ptr;
        if (desc.m_IndexBuffer != nullptr) {
            ptr = new GlVertexArrayIndexed(desc);
        }
        else {
            ptr = new GlVertexArray(desc);
        }
        m_VertexArrays.push_back(ptr);

        SR_CORE_POSTCONDITION(ptr != nullptr, "Vertex array is null");

        return ptr;
    }

    ConstantBuffer *GlContext::createConstantBuffer(const ConstantBufferDesc &desc) {
        SR_CORE_PRECONDITION(desc.m_Size <= getMaxConstantBufferSize(), "Constant buffer size is too large");
        SR_CORE_PRECONDITION(desc.m_Size > 0, "Constant buffer size is 0");

        ensureCurrentContext();
        auto ptr = new GlConstantBuffer(desc);
        m_ConstantBuffers.push_back(ptr);

        SR_CORE_POSTCONDITION(ptr != nullptr, "Constant buffer is null");

        return ptr;
    }

    FrameBuffer *GlContext::createFrameBuffer(const FrameBufferDesc &desc) {
        SR_CORE_PRECONDITION(!desc.m_ColorAttachments.empty(), "Framebuffer must have at least one color attachment");
        SR_CORE_PRECONDITION(desc.m_Width > 0, "Framebuffer width must be greater than 0");
        SR_CORE_PRECONDITION(desc.m_Height > 0, "Framebuffer height must be greater than 0");
        SR_CORE_PRECONDITION(desc.m_Width <= getMaxFramebufferWidth(), "Framebuffer width is too large");
        SR_CORE_PRECONDITION(desc.m_Height <= getMaxFramebufferHeight(), "Framebuffer height is too large");

        ensureCurrentContext();
        auto ptr = new GlFrameBuffer(desc);
        m_FrameBuffers.push_back(ptr);

        SR_CORE_POSTCONDITION(ptr != nullptr, "Failed to create framebuffer");

        return ptr;
    }

    Texture2D *GlContext::createTexture2D(const Texture2DDesc& desc) {
        SR_CORE_PRECONDITION(desc.m_Image != nullptr, "Texture image is null");
        SR_CORE_PRECONDITION(desc.m_Image->getChannelCount() == 1 ||
                             desc.m_Image->getChannelCount() == 2 ||
                             desc.m_Image->getChannelCount() == 3 ||
                             desc.m_Image->getChannelCount() == 4, "Texture image must have 1, 2, 3 or 4 channels");
        SR_CORE_PRECONDITION(desc.m_Image->getWidth() <= getMaxTexture2DSize(), "Texture width is too large");
        SR_CORE_PRECONDITION(desc.m_Image->getHeight() <= getMaxTexture2DSize(), "Texture height is too large");
        SR_CORE_PRECONDITION(desc.m_Image->getWidth() > 0, "Texture width must be greater than 0");
        SR_CORE_PRECONDITION(desc.m_Image->getHeight() > 0, "Texture height must be greater than 0");

        ensureCurrentContext();

        auto ptr = new GlTexture2D(desc);
        m_Textures2D.push_back(ptr);

        SR_CORE_POSTCONDITION(ptr != nullptr, "Failed to create texture");

        return ptr;
    }

    Sampler *GlContext::createSampler(const SamplerDesc &desc) {
        auto ptr = new GlSampler(desc);
        m_Samplers.push_back(ptr);

        SR_CORE_POSTCONDITION(ptr != nullptr, "Sampler is null");

        return ptr;
    }
}

