#pragma once

#include "Utils.hpp"
#include "ShaderModule.hpp"

namespace Syrius{

    struct SR_CORE_API ShaderDesc{
        ShaderModule* m_VertexShader    = nullptr;
        ShaderModule* m_FragmentShader  = nullptr;
    };

    class SR_CORE_API Shader{
    public:
        explicit Shader(const ShaderDesc& desc);

        virtual ~Shader();

        virtual void bind() = 0;

        [[nodiscard]] virtual uint64 getIdentifier() const = 0;

    private:

    };

}



