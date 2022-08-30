#pragma once

#include "../../../../include/SyriusCore/Context/Shader.hpp"
#include "D3D11Utils.hpp"
#include "D3D11ShaderModule.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11Shader: public Shader{
    public:
        D3D11Shader(const ShaderDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11Shader() override;

        void bind() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    public:
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;

        std::unordered_map<SR_SHADER_TYPE, ShaderModule*> m_ShaderModules;

        ID3D11VertexShader* m_VertexShader;
        ID3D11PixelShader* m_PixelShader;

    };

}

#endif

