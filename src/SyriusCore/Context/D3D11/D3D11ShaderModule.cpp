#include "D3D11ShaderModule.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11ShaderModule::D3D11ShaderModule(const ShaderModuleDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context)
    : ShaderModule(desc),
    m_Device(device),
    m_Context(context),
    m_ShaderBlob(nullptr),
    m_Flags(D3DCOMPILE_ENABLE_STRICTNESS){
        SR_CORE_PRECONDITION(desc.m_CodeType == SR_SHADER_CODE_HLSL, "D3D11 shader module only supports HLSL code");

#if defined(SR_DEBUG_MODE)
        m_Flags |= D3DCOMPILE_DEBUG;
#endif
        std::string shaderCode = desc.m_Code;
        if (desc.m_LoadType == SR_LOAD_FROM_FILE){
            shaderCode = readFile(desc.m_Code);
        }

        ID3DBlob* exceptionBlob = nullptr;
        SR_D3D11_CALL(D3DCompile(
            shaderCode.c_str(),
            shaderCode.length(),
            nullptr,
            nullptr,
            nullptr,
            desc.m_EntryPoint.c_str(),
            getShaderVersion(desc.m_Type).c_str(),
            m_Flags,
            0,
            &m_ShaderBlob,
            &exceptionBlob));
        if (exceptionBlob){
            auto msg = static_cast<char*>(exceptionBlob->GetBufferPointer());
            SR_CORE_WARNING("D3D11ShaderModule failed to compile shader, details: " + std::string(msg));
            exceptionBlob->Release();
        }

    }

    D3D11ShaderModule::~D3D11ShaderModule() {
        if (m_ShaderBlob) {
            m_ShaderBlob->Release();
        }
    }

    uint64 D3D11ShaderModule::getIdentifier() const {
        return reinterpret_cast<uint64>(m_ShaderBlob);
    }

    std::string D3D11ShaderModule::getShaderVersion(SR_SHADER_TYPE shaderType) {
        switch (shaderType) {
            case SR_SHADER_VERTEX:
                return "vs_5_0";
            case SR_SHADER_FRAGMENT:
                return "ps_5_0";
            default: {
                SR_CORE_WARNING("Invalid shader type given to D3D11 shader");
                return "";
            }
        }

    }
}
#endif
