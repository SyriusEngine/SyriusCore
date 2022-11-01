#include "../../../include/SyriusCore/Context/Context.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    template<typename T>
    inline void destroyObjects(std::vector<T*>& objects){
        for (auto object : objects){
            delete object;
        }
        objects.clear();
    }

    Context::Context(const ContextDesc& desc)
    : m_VerticalSync(false),
    m_Type(desc.m_API),
    m_DefaultFrameBuffer(nullptr){

    }

    Context::~Context() {
        destroyObjects(m_ShaderModules);
        destroyObjects(m_Shaders);
        destroyObjects(m_VertexDescriptions);
        destroyObjects(m_VertexBuffers);
        destroyObjects(m_IndexBuffers);
        destroyObjects(m_VertexArrays);
        destroyObjects(m_ConstantBuffers);
        destroyObjects(m_FrameBuffers);
        destroyObjects(m_Textures2D);
        destroyObjects(m_Samplers);
    }

    SR_SUPPORTED_API Context::getType() const {
        return m_Type;
    }

    bool Context::isVerticalSyncEnabled() const {
        return m_VerticalSync;
    }

    VertexDescription *Context::createVertexDescription() {
        return new VertexDescription();
    }

    FrameBuffer *Context::getDefaultFrameBuffer() {
        return m_DefaultFrameBuffer;
    }

    void Context::setClearColor(float r, float g, float b, float a, FrameBuffer *frameBuffer) {
        frameBuffer->setClearColor(r, g, b, a);
    }


    void Context::clear(FrameBuffer *frameBuffer) {
        frameBuffer->clear();
    }

    void Context::draw(VertexArray *vao) {
        vao->drawBuffers();
    }

    void Context::drawInstanced(VertexArray *vao, uint32 instanceCount) {
        SR_CORE_PRECONDITION(instanceCount > 0, "Instance count must be greater than 0");

        vao->drawBuffersInstanced(instanceCount);
    }

    void Context::onResize(uint32 width, uint32 height, FrameBuffer* frameBuffer) {
        SR_CORE_PRECONDITION(width > 0, "Width must be greater than 0");
        SR_CORE_PRECONDITION(height > 0, "Height must be greater than 0");
        SR_CORE_PRECONDITION(width <= getMaxFramebufferWidth(), "Width must be less than or equal to the maximum framebuffer width");
        SR_CORE_PRECONDITION(height <= getMaxFramebufferHeight(), "Height must be less than or equal to the maximum framebuffer height");

        frameBuffer->setSize(width, height);
    }

    void Context::bindDefaultFrameBuffer() {
        m_DefaultFrameBuffer->bind();
    }

    void Context::beginRenderPass(FrameBuffer *frameBuffer) {
        frameBuffer->bind();
        frameBuffer->clear();
    }

    void Context::endRenderPass(FrameBuffer *frameBuffer) {
        frameBuffer->unbind();
    }

    void Context::destroyShaderModule(ShaderModule *shaderModule) {
        m_ShaderModules.erase(std::remove(m_ShaderModules.begin(), m_ShaderModules.end(), shaderModule), m_ShaderModules.end());
        delete shaderModule;
    }

    void Context::destroyShader(Shader *shader) {
        m_Shaders.erase(std::remove(m_Shaders.begin(), m_Shaders.end(), shader), m_Shaders.end());
        delete shader;
    }

    void Context::destroyVertexDescription(VertexDescription *vertexDescription) {
        m_VertexDescriptions.erase(std::remove(m_VertexDescriptions.begin(), m_VertexDescriptions.end(), vertexDescription), m_VertexDescriptions.end());
        delete vertexDescription;
    }

    void Context::destroyVertexBuffer(VertexBuffer *vertexBuffer) {
        m_VertexBuffers.erase(std::remove(m_VertexBuffers.begin(), m_VertexBuffers.end(), vertexBuffer), m_VertexBuffers.end());
        delete vertexBuffer;
    }

    void Context::destroyIndexBuffer(IndexBuffer *indexBuffer) {
        m_IndexBuffers.erase(std::remove(m_IndexBuffers.begin(), m_IndexBuffers.end(), indexBuffer), m_IndexBuffers.end());
        delete indexBuffer;
    }

    void Context::destroyVertexArray(VertexArray* vertexArray){
        m_VertexArrays.erase(std::remove(m_VertexArrays.begin(), m_VertexArrays.end(), vertexArray), m_VertexArrays.end());
        delete vertexArray;
    }

    void Context::destroyConstantBuffer(ConstantBuffer* constantBuffer){
        m_ConstantBuffers.erase(std::remove(m_ConstantBuffers.begin(), m_ConstantBuffers.end(), constantBuffer), m_ConstantBuffers.end());
        delete constantBuffer;
    }

    void Context::destroyFrameBuffer(FrameBuffer* frameBuffer){
        m_FrameBuffers.erase(std::remove(m_FrameBuffers.begin(), m_FrameBuffers.end(), frameBuffer), m_FrameBuffers.end());
        delete frameBuffer;
    }

    void Context::destroyTexture2D(Texture2D* texture2D){
        m_Textures2D.erase(std::remove(m_Textures2D.begin(), m_Textures2D.end(), texture2D), m_Textures2D.end());
        delete texture2D;
    }

    void Context::destroySampler(Sampler *sampler) {
        m_Samplers.erase(std::remove(m_Samplers.begin(), m_Samplers.end(), sampler), m_Samplers.end());
        delete sampler;
    }


}