#pragma once

#include "../../../../include/SyriusCore/Context/Context.hpp"
#include "../../../../include/SyriusCore/Dependencies/imgui/imgui.h"
#include "../../Dependencies/imgui/imgui_impl_win32.h"
#include "../../Dependencies/imgui/imgui_impl_dx11.h"

#include "D3D11ShaderModule.hpp"
#include "D3D11Shader.hpp"
#include "D3D11VertexBuffer.hpp"
#include "D3D11IndexBuffer.hpp"
#include "D3D11VertexArray.hpp"
#include "D3D11ConstantBuffer.hpp"
#include "D3D11Texture2D.hpp"
#include "D3D11FrameBuffer.hpp"
#include "D3D11ColorAttachment.hpp"
#include "D3D11Sampler.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11Context: public Context{
    public:
        D3D11Context(HWND& hwnd, const ContextDesc& desc);

        ~D3D11Context() override;

        void swapBuffers() override;

        void setVerticalSynchronisation(bool enable) override;

        FramebufferSize getFramebufferSize() override;

        std::string getAPIVersion() override;

        std::string getDeviceName() override;

        std::string getDeviceVendor() override;

        std::string getShadingLanguageVersion() override;

        int32 getMaxFramebufferWidth() override;

        int32 getMaxFramebufferHeight() override;

        int32 getMaxFramebufferTextureAttachments() override;

        int32 getMaxTextureSlots() override;

        int32 getMaxTexture2DSize() override;

        int32 getMaxConstantBufferSize() override;

        int32 getMaxDepthBufferBits() override;

        ShaderModule* createShaderModule(const ShaderModuleDesc& desc) override;

        Shader* createShader(const ShaderDesc& desc) override;

        VertexBuffer* createVertexBuffer(const VertexBufferDesc& desc) override;

        IndexBuffer* createIndexBuffer(const IndexBufferDesc& desc) override;

        VertexArray* createVertexArray(const VertexArrayDesc& desc) override;

        ConstantBuffer* createConstantBuffer(const ConstantBufferDesc& desc) override;

        FrameBuffer* createFrameBuffer(const FrameBufferDesc& desc) override;

        Texture2D* createTexture2D(const Texture2DDesc& desc) override;

        Sampler* createSampler(const SamplerDesc& desc) override;

    protected:

        void createImGuiContext() override;

        void destroyImGuiContext() override;

        void onImGuiBegin() override;

        void onImGuiEnd() override;

    private:
        HWND& m_Hwnd;

        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;
        IDXGISwapChain* m_SwapChain;

        ImGuiContext *m_ImGuiContext;

        D3D_FEATURE_LEVEL m_D3DVersion;

    };

}

#endif

