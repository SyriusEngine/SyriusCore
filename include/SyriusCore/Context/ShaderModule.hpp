#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_CORE_API ShaderModuleDesc{
        std::string code; // shader code
        SR_SHADER_TYPE shaderType           = SR_SHADER_NONE;
        std::string entryPoint              = "main";
        SR_SHADER_LANGUAGE_TYPE language    = SR_SHADER_LANGUAGE_NONE;
    };

    struct SR_CORE_API ShaderModuleFileDesc{
        std::string filePath;
        SR_SHADER_TYPE shaderType           = SR_SHADER_NONE;
        std::string entryPoint              = "main";
        SR_SHADER_LANGUAGE_TYPE language    = SR_SHADER_LANGUAGE_NONE;
    };

    class SR_CORE_API ShaderModule{
    public:
        explicit ShaderModule(const ShaderModuleDesc& desc);

        explicit ShaderModule(const ShaderModuleFileDesc& desc);

        virtual ~ShaderModule();

        [[nodiscard]] virtual u64 getIdentifier() const = 0;

        [[nodiscard]] SR_SHADER_TYPE getShaderModuleType() const;

        [[nodiscard]] const std::string& getEntryPoint() const;

    protected:

        static std::string readFile(const std::string& fileName);

        static std::string readFileBinary(const std::string& fileName);

    protected:
        const SR_SHADER_TYPE m_ShaderType;
        const std::string m_EntryPoint;

    };
}

