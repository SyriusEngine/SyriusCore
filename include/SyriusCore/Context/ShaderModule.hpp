#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_CORE_API ShaderModuleDesc{
        std::string code;     // can be actual code in a string or a file path
        SR_SHADER_TYPE shaderType           = SR_SHADER_VERTEX;
        std::string entryPoint        = "main";
        SR_LOAD_TYPE loadType         = SR_LOAD_FROM_FILE;
        SR_SHADER_CODE_TYPE codeType  = SR_SHADER_CODE_NONE;
    };

    class SR_CORE_API ShaderModule{
    public:
        explicit ShaderModule(const ShaderModuleDesc& desc);

        virtual ~ShaderModule();

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

        [[nodiscard]] SR_SHADER_TYPE getShaderModuleType() const;

        [[nodiscard]] const std::string& getEntryPoint() const;

    protected:

        static std::string readFile(const std::string& fileName);

        static std::string readFileBinary(const std::string& fileName);

    private:
        SR_SHADER_TYPE m_ShaderType;
        std::string m_EntryPoint;

    };
}

