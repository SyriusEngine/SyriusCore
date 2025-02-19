#pragma once

#include "../../../../include/SyriusCore/Context/ShaderModule.hpp"
#include "D3D11Utils.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    class D3D11ShaderModule: public ShaderModule{
    public:
        D3D11ShaderModule(const ShaderModuleDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        D3D11ShaderModule(const ShaderModuleFileDesc& desc, ID3D11Device* device, ID3D11DeviceContext* context);

        ~D3D11ShaderModule() override;

        [[nodiscard]] u64 getIdentifier() const override;

    private:

        static std::string getShaderVersion(SR_SHADER_TYPE shaderType);

        void loadShader(const std::string& code);

    private:
        ID3DBlob* m_ShaderBlob;
        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_Context;
    };

}

#endif
