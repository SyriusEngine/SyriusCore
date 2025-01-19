#include "GlContext.hpp"

namespace Syrius{

    // keep track of with gl context is currently active
    u64 GlContext::m_ActiveContextID = 0;
    u32 GlContext::m_ContextCount = 0;

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
        m_CubeMaps.clear();

        terminateGlad();
    }

    ResourceView<ShaderModule> GlContext::createShaderModule(const ShaderModuleDesc &desc) {
        m_ShaderModules.emplace_back(new GlShaderModule(desc));
        return createResourceView(m_ShaderModules.back());
    }

    ResourceView<ShaderModule> GlContext::createShaderModule(const ShaderModuleFileDesc &desc) {
        m_ShaderModules.emplace_back(new GlShaderModule(desc));
        return createResourceView(m_ShaderModules.back());
    }

    ResourceView<Shader> GlContext::createShader(const ShaderDesc &desc) {
        m_Shaders.emplace_back(new GlShader(desc));
        return createResourceView(m_Shaders.back());
    }

    ResourceView<VertexBuffer> GlContext::createVertexBuffer(const VertexBufferDesc &desc) {
        m_VertexBuffers.emplace_back(new GlVertexBuffer(desc, m_DeviceLimits));
        return createResourceView(m_VertexBuffers.back());
    }

    ResourceView<IndexBuffer> GlContext::createIndexBuffer(const IndexBufferDesc &desc) {
        m_IndexBuffers.emplace_back(new GlIndexBuffer(desc, m_DeviceLimits));
        return createResourceView(m_IndexBuffers.back());
    }

    ResourceView<VertexArray> GlContext::createVertexArray(const VertexArrayDesc &desc) {
        VertexArray* ptr;
        if (desc.indexBuffer != nullptr) {
            ptr =  new GlVertexArrayIndexed(desc);
        }
        else {
            ptr =  new GlVertexArray(desc);
        }
        m_VertexArrays.emplace_back(ptr);
        return createResourceView(m_VertexArrays.back());
    }

    ResourceView<ConstantBuffer> GlContext::createConstantBuffer(const ConstantBufferDesc &desc) {
        m_ConstantBuffers.emplace_back(new GlConstantBuffer(desc, m_DeviceLimits));
        return createResourceView(m_ConstantBuffers.back());
    }

    ResourceView<Texture2D> GlContext::createTexture2D(const Texture2DDesc& desc) {
        m_Textures2D.emplace_back(new GlTexture2D(desc, m_DeviceLimits));
        return createResourceView(m_Textures2D.back());
    }

    ResourceView<Texture2D> GlContext::createTexture2D(const Texture2DImageDesc &desc) {
        m_Textures2D.emplace_back(new GlTexture2D(desc, m_DeviceLimits));
        return createResourceView(m_Textures2D.back());
    }

    ResourceView<Sampler> GlContext::createSampler(const SamplerDesc &desc) {
        m_Samplers.emplace_back(new GlSampler(desc));
        return createResourceView(m_Samplers.back());
    }

    ResourceView<FrameBuffer> GlContext::createFrameBuffer(const ResourceView<FrameBufferLayout> &desc) {
        m_FrameBuffers.emplace_back(new GlFrameBuffer(desc, m_DeviceLimits));
        return createResourceView(m_FrameBuffers.back());
    }

    ResourceView<CubeMap> GlContext::createCubeMap(const ResourceView<Syrius::CubeMapLayout> &desc) {
        m_CubeMaps.emplace_back(new GlCubeMap(desc, m_DeviceLimits));
        return createResourceView(m_CubeMaps.back());
    }

    void GlContext::initGl(const ContextDesc& desc) {
        initGlad();

        auto defaultFbDesc = createFrameBufferLayout();

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

        m_DeviceLimits = createUP<GlDeviceLimits>();

        auto ptr = new GlDefaultFrameBuffer(defaultFbDesc, m_DeviceLimits);
        m_FrameBuffers.emplace_back(ptr);

        SR_PRECONDITION(!m_FrameBuffers.empty(), "Default framebuffer not created");
    }

    void GlContext::terminateGl() {
        terminateGlad();
    }

    void GlContext::initGlad() {
        if (m_ContextCount == 0) {
            const i32 version = gladLoaderLoadGL();

            SR_LOG_THROW_IF_FALSE(version != 0, "GlContext", "Failed to initialize Glad")

            #if defined(SR_DEBUG)
                const i32 major = GLAD_VERSION_MAJOR(version);
                const i32 minor = GLAD_VERSION_MINOR(version);
                SR_LOG_INFO("GlContext", "Glad Initialized for OpenGL %i.%i", major, minor)
                if (major < 4 || (major == 4 && minor < 6)){
                    SR_LOG_WARNING("GlContext", "SyriusCore is designed around OpenGL version is 4.6. Some features may not be available with version  %i.%i", major, minor)
                }
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

                #if defined(SR_COMPILER_MSVC)
                    auto func = (GLDEBUGPROC) CoreLogger::openGLMessageCallback;
                    glDebugMessageCallback(func, nullptr);
                #else
                    glDebugMessageCallback(CoreLogger::openGLMessageCallback, nullptr);
                #endif
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

