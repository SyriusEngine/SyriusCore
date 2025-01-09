#pragma once

#include "../../../../include/SyriusCore/Context/VertexArray.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11VertexArray: public VertexArray{
    public:
        D3D11VertexArray(const VertexArrayDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11VertexArray() override;

        void bind() override;

        void unbind() override;

        void drawBuffers() override;

        void drawBuffersInstanced(u32 instanceCount) override;

        void setDrawMode(SR_DRAW_MODE drawMode) override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11InputLayout* m_InputLayout;

        D3D11_PRIMITIVE_TOPOLOGY m_D3d11DrawMode;
    };

    class D3D11VertexArrayIndexed: public VertexArrayIndexed{
    public:
        D3D11VertexArrayIndexed(const VertexArrayDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11VertexArrayIndexed() override;

        void bind() override;

        void unbind() override;

        void drawBuffers() override;

        void drawBuffersInstanced(u32 instanceCount) override;

        void setDrawMode(SR_DRAW_MODE drawMode) override;

    private:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        ID3D11InputLayout* m_InputLayout;

        D3D11_PRIMITIVE_TOPOLOGY m_D3d11DrawMode;
    };

}

#endif

