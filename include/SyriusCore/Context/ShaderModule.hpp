#pragma once

#include "Utils.hpp"

namespace Syrius{

    struct SR_API ShaderModuleDesc{
        std::string m_Code;     // can be actual code in a string or a file path
        uint32 m_CodeLength = 0;
        SR_SHADER_TYPE m_Type;
        std::string m_EntryPoint;
        SR_LOAD_TYPE m_LoadType;
        SR_SHADER_CODE_TYPE m_CodeType;
    };

    class SR_API ShaderModule{
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

