#include "GlContext.hpp"

namespace Syrius{

    uint64 GlContext::m_ActiveContextID = 0;

    GlContext::GlContext(const ContextDesc& desc)
    : Context(desc),
    m_ID(generateID()){
        m_ActiveContextID = m_ID;
    }

    template<typename T>
    inline void clearVec(std::vector<T>& vec){
        for (auto& item : vec){
            item.destroy();
        }
    }

    GlContext::~GlContext() {
        // every GL object should be destroyed/released before the context is terminated
        clearVec(m_ShaderModules);
        clearVec(m_Shaders);
        clearVec(m_VertexBuffers);
        clearVec(m_IndexBuffers);
        clearVec(m_VertexDescriptions);
        clearVec(m_Textures2D);
        clearVec(m_Samplers);
        clearVec(m_ConstantBuffers);
        clearVec(m_VertexArrays);
        clearVec(m_FrameBuffers);
        clearVec(m_FrameBufferDescriptions);

        CoreCommand::terminateGlad();
    }

    void GlContext::initGl(const ContextDesc& desc) {
        CoreCommand::initGlad();

        auto defaultFbDesc = createFrameBufferDescription();
        ViewportDesc viewportDesc;
        viewportDesc.width = desc.backBufferWidth;
        viewportDesc.height = desc.backBufferHeight;
        defaultFbDesc->addViewportDesc(viewportDesc);
        ColorAttachmentDesc colorAttachmentDesc;
        colorAttachmentDesc.width = desc.backBufferWidth;
        colorAttachmentDesc.height = desc.backBufferHeight;
        defaultFbDesc->addColorAttachmentDesc(colorAttachmentDesc);

        auto ptr = new GlDefaultFrameBuffer(defaultFbDesc);
        m_FrameBuffers.emplace_back(ptr);
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
        auto ptr = new GlConstantBuffer(desc);
        m_ConstantBuffers.emplace_back(ptr);
        return m_ConstantBuffers.back().createView();
    }

    ResourceView<Texture2D> GlContext::createTexture2D(const Texture2DDesc& desc) {
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

}

