#include "GlContext.hpp"

namespace Syrius{

    // keep track of with gl context is currently active
    uint64 GlContext::m_ActiveContextID = 0;
    uint32 GlContext::m_ContextCount = 0;

    GlContext::GlContext(const ContextDesc& desc):
    Context(desc),
    m_ID(generateID()){
        m_ActiveContextID = m_ID;
    }

    GlContext::~GlContext() {
        // every GL object should be destroyed/released before GL functions are unloaded
        m_ShaderModules.clear();
        m_Shaders.clear();
        m_VertexBuffers.clear();
        m_IndexBuffers.clear();
        m_VertexDescriptions.clear();
        m_Textures2D.clear();
        m_Samplers.clear();
        m_ConstantBuffers.clear();
        m_VertexArrays.clear();
        m_FrameBuffers.clear();
        m_FrameBufferDescriptions.clear();
        m_Cubemaps.clear();

        terminateGlad();
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

    ResourceView<ShaderModule> GlContext::createShaderModule(const ShaderModuleDesc &desc) {
        auto ptr = new GlShaderModule(desc);
        m_ShaderModules.emplace_back(ptr);
        return m_ShaderModules.back().createView();
    }

    ResourceView<Shader> GlContext::createShader(const ShaderDesc &desc) {
        auto ptr = new GlShader(desc);
        m_Shaders.emplace_back(ptr);
        return m_Shaders.back().createView();
    }

    ResourceView<VertexBuffer> GlContext::createVertexBuffer(const VertexBufferDesc &desc) {
        auto ptr = new GlVertexBuffer(desc);
        m_VertexBuffers.emplace_back(ptr);
        return m_VertexBuffers.back().createView();
    }

    ResourceView<IndexBuffer> GlContext::createIndexBuffer(const IndexBufferDesc &desc) {
        auto ptr = new GlIndexBuffer(desc);
        m_IndexBuffers.emplace_back(ptr);
        return m_IndexBuffers.back().createView();
    }

    ResourceView<VertexArray> GlContext::createVertexArray(const VertexArrayDesc &desc) {
        VertexArray* ptr;
        if (desc.indexBuffer.isValid()) {
            ptr =  new GlVertexArrayIndexed(desc);
        }
        else {
            ptr =  new GlVertexArray(desc);
        }
        m_VertexArrays.emplace_back(ptr);
        return m_VertexArrays.back().createView();
    }

    ResourceView<ConstantBuffer> GlContext::createConstantBuffer(const ConstantBufferDesc &desc) {
        SR_CORE_PRECONDITION(desc.size <= getMaxConstantBufferSize(), "Constant buffer size (%llu) exceeds maximum allowed size (%llu)", desc.size, getMaxConstantBufferSize());

        auto ptr = new GlConstantBuffer(desc);
        m_ConstantBuffers.emplace_back(ptr);
        return m_ConstantBuffers.back().createView();
    }

    ResourceView<Texture2D> GlContext::createTexture2D(const Texture2DDesc& desc) {
        SR_CORE_PRECONDITION(desc.width * desc.height * getTextureFormatChannelCount(desc.format) <= getMaxTexture2DSize(), "Texture size (%llu) exceeds maximum allowed size (%llu)", desc.width * desc.height * getTextureFormatChannelCount(desc.format), getMaxTexture2DSize());

        auto ptr = new GlTexture2D(desc);
        m_Textures2D.emplace_back(ptr);

        return m_Textures2D.back().createView();
    }

    ResourceView<Texture2D> GlContext::createTexture2D(const Texture2DImageDesc &desc) {
        auto ptr = new GlTexture2D(desc);
        m_Textures2D.emplace_back(ptr);
        return m_Textures2D.back().createView();
    }

    ResourceView<Sampler> GlContext::createSampler(const SamplerDesc &desc) {
        auto ptr = new GlSampler(desc);
        m_Samplers.emplace_back(ptr);
        return m_Samplers.back().createView();
    }

    ResourceView<FrameBuffer> GlContext::createFrameBuffer(const ResourceView<FrameBufferDescription> &desc) {
        auto ptr = new GlFrameBuffer(desc);
        m_FrameBuffers.emplace_back(ptr);
        return m_FrameBuffers.back().createView();
    }

    ResourceView<Cubemap> GlContext::createCubemap(const CubemapDesc &desc) {
        auto ptr = new GlCubemap(desc);
        m_Cubemaps.emplace_back(ptr);
        return m_Cubemaps.back().createView();
    }

    ResourceView<Cubemap> GlContext::createCubemap(const CubemapImageDesc &desc) {
        auto ptr = new GlCubemap(desc);
        m_Cubemaps.emplace_back(ptr);
        return m_Cubemaps.back().createView();
    }

    void GlContext::initGl(const ContextDesc& desc) {
        initGlad();

        auto defaultFbDesc = createFrameBufferDescription();

        ViewportDesc viewportDesc;
        viewportDesc.width = desc.backBufferWidth;
        viewportDesc.height = desc.backBufferHeight;
        defaultFbDesc->addViewportDesc(viewportDesc);

        ColorAttachmentDesc colorAttachmentDesc;
        colorAttachmentDesc.width = desc.backBufferWidth;
        colorAttachmentDesc.height = desc.backBufferHeight;
        defaultFbDesc->addColorAttachmentDesc(colorAttachmentDesc);

        if (desc.enableDepthTest or desc.enableStencilTest){
            DepthStencilAttachmentDesc dsaDesc;
            dsaDesc.width = desc.backBufferWidth;
            dsaDesc.height = desc.backBufferHeight;
            dsaDesc.enableDepthTest = desc.enableDepthTest;
            dsaDesc.enableStencilTest = desc.enableStencilTest;
            defaultFbDesc->addDepthStencilAttachmentDesc(dsaDesc);
        }

        auto ptr = new GlDefaultFrameBuffer(defaultFbDesc);
        m_FrameBuffers.emplace_back(ptr);
    }

    void GlContext::terminateGl() {
        terminateGlad();
    }

    void GlContext::initGlad() {
        if (m_ContextCount == 0) {
            int32 version = gladLoaderLoadGL();
#if defined(SR_CORE_DEBUG)
            if (version > 0){
                int32 major = GLAD_VERSION_MAJOR(version);
                int32 minor = GLAD_VERSION_MINOR(version);
                SR_CORE_MESSAGE("OpenGL initialized with version: %i.%i", major, minor)
                if (major < 4 || (major == 4 && minor < 5)){
                    SR_CORE_WARNING("Supported OpenGL version is 4.5. Some features may not be available with version  %i.%i", major, minor)
                }
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#if defined(SR_COMPILER_MSVC)
                auto func = (GLDEBUGPROC) DebugMessageHandler::pushOpenGLMessageCallback;
                glDebugMessageCallback(func, nullptr);
#else
                glDebugMessageCallback(DebugMessageHandler::pushOpenGLMessageCallback, nullptr);
#endif
            }
            else {
                SR_CORE_EXCEPTION("Failed to initialize OpenGL");
            }
#endif
        }
        m_ContextCount++;
    }

    void GlContext::terminateGlad() {
        m_ContextCount--;
        if (m_ContextCount == 0) {
            gladLoaderUnloadGL();
        }
    }
}

