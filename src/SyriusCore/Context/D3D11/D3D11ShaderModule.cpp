#include "D3D11ShaderModule.hpp"

#if defined(SR_PLATFORM_WIN64)

namespace Syrius{

    D3D11ShaderModule::D3D11ShaderModule(const ShaderModuleDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context):
    ShaderModule(desc),
    m_Device(device),
    m_Context(context),
    m_ShaderBlob(nullptr){
        SR_PRECONDITION(desc.language == SR_SHADER_LANGUAGE_HLSL, "D3D11 shader module only supports HLSL code, provided language %i", desc.language);

        loadShader(desc.code);

        SR_POSTCONDITION(m_ShaderBlob != nullptr, "Failed to load shader")
    }

    D3D11ShaderModule::D3D11ShaderModule(const ShaderModuleFileDesc &desc, ID3D11Device *device, ID3D11DeviceContext *context):
    ShaderModule(desc),
    m_Device(device),
    m_Context(context),
    m_ShaderBlob(nullptr){
        SR_PRECONDITION(desc.language == SR_SHADER_LANGUAGE_HLSL, "D3D11 shader module only supports HLSL code, provided language %i", desc.language);

        auto code = readFile(desc.filePath);
        loadShader(code);

        SR_POSTCONDITION(m_ShaderBlob != nullptr, "Failed to load shader")
    }

    D3D11ShaderModule::~D3D11ShaderModule() {
        if (m_ShaderBlob) {
            m_ShaderBlob->Release();
        }
    }

    u64 D3D11ShaderModule::getIdentifier() const {
        return reinterpret_cast<u64>(m_ShaderBlob);
    }

    std::string D3D11ShaderModule::getShaderVersion(SR_SHADER_TYPE shaderType) {
        switch (shaderType) {
            case SR_SHADER_VERTEX:
                return "vs_5_0";
            case SR_SHADER_FRAGMENT:
                return "ps_5_0";
            default: {
                SR_LOG_THROW("D3D11ShaderModule", "Invalid shader type (%i) given to D3D11 shader", shaderType);
                return "";
            }
        }

    }

    void D3D11ShaderModule::loadShader(const std::string &code) {
        SR_PRECONDITION(m_ShaderBlob == nullptr, "Shader blob already exists")
        SR_PRECONDITION(!code.empty(), "Shader code is empty")

        u32 flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(SR_DEBUG)
        flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#else
        flags |= D3DCOMPILE_OPTIMIZATION_LEVEL2;
#endif
        ID3DBlob* exceptionBlob = nullptr;
        SR_CORE_D3D11_CALL(D3DCompile(
                code.c_str(),
                code.length(),
                nullptr,
                nullptr,
                nullptr,
                m_EntryPoint.c_str(),
                getShaderVersion(m_ShaderType).c_str(),
                flags,
                0,
                &m_ShaderBlob,
                &exceptionBlob));

        if (exceptionBlob){
            const auto msg = createUP<char[]>(exceptionBlob->GetBufferSize() + 1);
            memcpy(msg.get(), exceptionBlob->GetBufferPointer(), exceptionBlob->GetBufferSize());
            SR_LOG_THROW("D3D11ShaderModule", "Failed to compile shader, type = %s, error = %s", getShaderTypeString(m_ShaderType).c_str(), msg.get());
        }
    }

}
#endif
