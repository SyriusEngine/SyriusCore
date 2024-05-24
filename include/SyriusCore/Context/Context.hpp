#pragma once

#include "../Utils/SyriusCoreInclude.hpp"
#include "../Utils/Resource.hpp"
#include "DeviceLimits.hpp"
#include "ShaderModule.hpp"
#include "Shader.hpp"
#include "Viewport.hpp"
#include "VertexLayout.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture2D.hpp"
#include "ConstantBuffer.hpp"
#include "Sampler.hpp"
#include "FrameBufferLayout.hpp"
#include "FrameBuffer.hpp"
#include "ColorAttachment.hpp"
#include "DepthStencilAttachment.hpp"
#include "CubeMapLayout.hpp"
#include "CubeMap.hpp"

namespace Syrius{

    class SyriusWindow;

    struct SR_CORE_API ContextDesc{
        uint8 depthBits        = 24;
        uint8 stencilBits      = 8;
        uint8 redBits          = 8;
        uint8 blueBits         = 8;
        uint8 greenBits        = 8;
        uint8 alphaBits        = 8;
        uint32 backBufferWidth = 0; // if 0, will be set to window width
        uint32 backBufferHeight= 0; // if 0, will be set to window height
        bool enableDepthTest   = false;
        bool enableStencilTest = false;
        SR_SUPPORTED_API api   = SR_API_OPENGL;
    };

    struct SR_CORE_API FramebufferSize{
        uint32 m_Width  = 0;
        uint32 m_Height = 0;
    };

    class SR_CORE_API Context{
    public:
        Context() = delete;

        Context(const Context&) = delete;

        virtual ~Context();

        /**
         * @brief: Commands the swap chain to present a back buffer to the screen. If multiple back buffers are used,
         *         this function will swap the back buffer that was last presented to the screen with the front buffer.
         */
        virtual void swapBuffers() = 0;

        /**
         * @brief: Enables or disables vertical synchronisation. When enabled, the swap chain will wait for the
         *        vertical blanking period before presenting the back buffer to the screen. This will prevent screen
         *        tearing but will also limit the frame rate to the refresh rate of the monitor.
         * @param enable - true to enable vertical synchronisation, false to disable it.
         * @note: This function is not supported on all APIs. If the API does not support vertical synchronisation, this
         *        function will do nothing. In case of OpenGL, the necessary extensions must be available on your device
         *        for this function to work.
         */
        virtual void setVerticalSynchronisation(bool enable) = 0;

        [[nodiscard]] SR_SUPPORTED_API getType() const;

        [[nodiscard]] bool isVerticalSyncEnabled() const;

        ResourceView<FrameBuffer> getDefaultFrameBuffer();

        virtual FramebufferSize getFramebufferSize() = 0;

        /**
         * @brief: Creates a new shader module. The description includes a field that should contain the shader code.
         * @param desc - the description of the shader module.
         * @return: a resource view to the created shader module.
         */
        virtual ResourceView<ShaderModule> createShaderModule(const ShaderModuleDesc& desc) = 0;

        /**
         * @brief: Creates a new shader module from a file. The description includes a field that should contain the
         *         path to the file.
         * @param desc - the description of the shader module.
         * @return: a resource view to the created shader module.
         */
        virtual ResourceView<ShaderModule> createShaderModule(const ShaderModuleFileDesc& desc) = 0;

        /**
         * @brief: Creates a new shader. The description should contain all shader modules that should be linked together
         *        to create the shader. Minimally, the vertex and fragment shader modules should be included.
         * @param desc - the description of the shader.
         * @return: a resource view to the created shader.
         */
        virtual ResourceView<Shader> createShader(const ShaderDesc& desc) = 0;

        /**
         * @brief: Creates a new vertex layout. The vertex layout is used to describe the layout of the vertex buffer
         *        that is used by the vertex shader.
         * @return: a resource view to the created vertex layout.
         */
        ResourceView<VertexLayout> createVertexLayout();

        /**
         * @brief: Creates a new vertex buffer. Can be used to store vertex data on the graphics device.
         * @param desc - the description of the vertex buffer.
         * @return: a resource view to the created vertex buffer.
         */
        virtual ResourceView<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc) = 0;

        /**
         * @brief: Creates a new index buffer.
         * @param desc - the description of the index buffer.
         * @return: a resource view to the created index buffer.
         */
        virtual ResourceView<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc) = 0;

        /**
         * @brief: Creates a new vertex array. The vertex array is used to bind a vertex buffer and an index buffer to the
         *        input assembler stage of the graphics pipeline.
         * @param desc - the description of the vertex array.
         * @return: a resource view to the created vertex array.
         */
        virtual ResourceView<VertexArray> createVertexArray(const VertexArrayDesc& desc) = 0;

        /**
         * @brief: Creates a new constant buffer. Constant buffers can be used to store constant data on the graphics
         *       device. This data can be accessed by the shaders. Note that constant buffers can be bound to a single
         *       shader stage. The choice of shader stage should be determined before creating the constant buffer as
         *       it cannot be changed afterwards.
         * @param desc - the description of the constant buffer.
         * @return: a resource view to the created constant buffer.
         */
        virtual ResourceView<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc& desc) = 0;

        /**
         * @brief: Creates a new 2 Dimensional texture. This texture creation function will fill the texture data region
         *        with the data provided in the description. The size of the data should be described in the description.
         *        If an empty texture is desired, the data field should be set to nullptr.
         * @param desc - the description of the texture 2D.
         * @return: a resource view to the created texture 2D.
         */
        virtual ResourceView<Texture2D> createTexture2D(const Texture2DDesc& desc) = 0;

        /**
         * @brief: Creates a new 2 Dimensional texture. This texture creation function will load the texture data from
         *         an image resource. If the texture data should be loaded from a file, the image resource should be
         *         created using the createImage function. The image resource should be provided in the description.
         *
         * @param desc - the description of the texture 2D.
         * @return: a resource view to the created texture 2D.
         */
        virtual ResourceView<Texture2D> createTexture2D(const Texture2DImageDesc& desc) = 0;

        /**
         * @brief: Creates a new sampler. The sampler object describes how the sampling should be done when a shader
         *       accesses a texture.
         * @param desc - the description of the sampler.
         * @return: a resource view to the created sampler.
         */
        virtual ResourceView<Sampler> createSampler(const SamplerDesc& desc) = 0;

        /**
         * @brief: Creates a new frame buffer layout. The layout describes the number of attachments that the frame buffer
         *       will have. The attachments can be color attachments, depth attachments, or stencil attachments and
         *       viewports. The frame buffer layout is used to create a frame buffer.
         * @return: a resource view to the created frame buffer layout.
         */
        ResourceView<FrameBufferLayout> createFrameBufferLayout();

        /**
         * @brief: Creates a new frame buffer. The frame buffer is a collection of attachments that can be used to render
         *       to. The benefit of using a frame buffer is that the render output can be used in further operations.
         * @param desc - the description of the frame buffer.
         * @return: a resource view to the created frame buffer.
         */
        virtual ResourceView<FrameBuffer> createFrameBuffer(const ResourceView<FrameBufferLayout>& desc) = 0;

        /**
         * @brief: Creates a new CubeMap layout object. The CubeMap layout object is used to describe the layout of a
         *         cubeMap texture by adding various faces to it. The faces can be added using the addFace function or can be
         *         loaded from files using the addFaceFromFile function.
         * @param width the width of each face of the cubemap
         * @param height the height of each face of the cubemap
         * @param format the format of the cubemap
         * @return a resource view to the created CubeMapLayout object
         */
        ResourceView<CubeMapLayout> createCubeMapLayout(uint32 width, uint32 height, SR_TEXTURE_FORMAT format);

        /**
         * @brief: Creates a new CubeMap object. A CubeMap is a collection of 6 textures that are used to create a cube,
         *         essentially a 3D texture of 6 individual 2D textures.
         * @param desc a CubeMapLayout object that describes the layout of the cubemap
         * @return a resource view to the created CubeMap object
         */
        virtual ResourceView<CubeMap> createCubeMap(const ResourceView<CubeMapLayout>& desc) = 0;

        void onResize(uint32 width, uint32 height);

        virtual void beginRenderPass(const ResourceView<FrameBuffer>& frameBuffer);

        virtual void beginRenderPass();

        virtual void endRenderPass(const ResourceView<FrameBuffer>& frameBuffer);

        virtual void endRenderPass();

        /**
         * @brief: Clears the back buffer with the color specified in the clear color. The clear color is set using the
         *       setClearColor function.
         */
        void clear();

        /**
         * @brief: Sets the clear color of the back buffer. The clear color is used when the clear function is called.
         * @param r - the red component of the clear color.
         * @param g - the green component of the clear color.
         * @param b - the blue component of the clear color.
         * @param a - the alpha component of the clear color. Default is 1.0f.
         */
        void setClearColor(float r, float g, float b, float a = 1.0f);

        /**
         * @brief: Retrieves the clear color of the back buffer. This is a 4 element array where the first element is the
         *       red component, the second element is the green component, the third element is the blue component, and
         *       the fourth element is the alpha component.
         * @return: a pointer to the clear color array.
         */
        float* getClearColor();

        /**
         * @brief: Given a valid vertex array, this function will send the necessary commands to the graphics device
         *        to draw the data stored in the array. The data will be drawn using the currently bound shader.
         * @param vertexArray
         */
        void draw(const ResourceView<VertexArray>& vertexArray);

        /**
         * @brief: Similar to the draw function, but this function will draw multiple instances of the data stored in the
         *       vertex array. The number of instances to draw is specified by the instanceCount parameter.
         * @param vertexArray
         * @param instanceCount - the number of instances to draw.
         */
        void drawInstanced(const ResourceView<VertexArray>& vertexArray, uint32 instanceCount);

        /**
         * @brief: Enables or disables depth testing for the default framebuffer. When enabled, the depth buffer will be
         * used to determine which fragments should be drawn and which should be discarded.
         * @param enable - true to enable depth testing, false to disable it.
         */
        void enableDepthTest(bool enable);

        /**
         * @brief: The DeviceLimits object contains information about the capabilities of the graphics device that the
         *       context is running on. This information can be used to determine the maximum number of textures that
         *       can be bound to a shader, the maximum number of vertex attributes that can be used, etc.
         */
        [[nodiscard]] const Resource<DeviceLimits>& getDeviceLimits() const;

        [[nodiscard]] uint32 getWidth() const;

        [[nodiscard]] uint32 getHeight() const;

        void destroyShaderModule(const ResourceView<ShaderModule>& shaderModule);

        void destroyShader(const ResourceView<Shader>& shader);

        void destroyVertexLayout(const ResourceView<VertexLayout>& vertexLayout);

        void destroyVertexBuffer(const ResourceView<VertexBuffer>& vertexBuffer);

        void destroyIndexBuffer(const ResourceView<IndexBuffer>& indexBuffer);

        void destroyVertexArray(const ResourceView<VertexArray>& vertexArray);

        void destroyConstantBuffer(const ResourceView<ConstantBuffer>& constantBuffer);

        void destroyTexture2D(const ResourceView<Texture2D>& texture2D);

        void destroySampler(const ResourceView<Sampler>& sampler);

        void destroyFrameBufferLayout(const ResourceView<FrameBufferLayout>& frameBufferLayout);

        void destroyFrameBuffer(const ResourceView<FrameBuffer>& frameBuffer);

        void destroyCubeMapLayout(const ResourceView<CubeMapLayout>& cubeMapLayout);

        void destroyCubeMap(const ResourceView<CubeMap>& cubeMap);

    protected:

        explicit Context(const ContextDesc& desc);

        virtual void createImGuiContext() = 0;

        virtual void destroyImGuiContext() = 0;

        virtual void onImGuiBegin() = 0;

        virtual void onImGuiEnd() = 0;

    protected:

        friend SyriusWindow;

    protected:

        bool m_VerticalSync;

        Resource<DeviceLimits> m_DeviceLimits;

        /*
         * The Context class is responsible for managing the lifetime of the created resources. This is because a lot
         * of these resources depend on API specific objects that are managed by the context. In case of D3D11, the
         * D3D11Context object manages the Device, DeviceContext, SwapChain, etc. These objects are used by all
         * created resources. Should the context be destroyed before some of these resources, the resources would
         * be left with dangling pointers to the API specific objects and will cause a crash when trying to use them.
         * Therefore, the context is responsible for destroying all the resources that were created using it.
         */
        std::vector<Resource<ShaderModule>> m_ShaderModules;
        std::vector<Resource<Shader>> m_Shaders;
        std::vector<Resource<VertexLayout>> m_VertexDescriptions;
        std::vector<Resource<VertexBuffer>> m_VertexBuffers;
        std::vector<Resource<IndexBuffer>> m_IndexBuffers;
        std::vector<Resource<VertexArray>> m_VertexArrays;
        std::vector<Resource<ConstantBuffer>> m_ConstantBuffers;
        std::vector<Resource<Texture2D>> m_Textures2D;
        std::vector<Resource<Sampler>> m_Samplers;
        std::vector<Resource<FrameBufferLayout>> m_FrameBufferDescriptions;
        std::vector<Resource<FrameBuffer>> m_FrameBuffers; // framebuffer at location 0 is the default framebuffer
        std::vector<Resource<CubeMapLayout>> m_CubeMapLayouts;
        std::vector<Resource<CubeMap>> m_CubeMaps;


    private:

        SR_SUPPORTED_API m_Type;

    };

}