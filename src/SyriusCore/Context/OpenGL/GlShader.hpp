#pragma once

#include "GlUtils.hpp"
#include "GlShaderModule.hpp"
#include "../../../../include/SyriusCore/Context/Shader.hpp"

namespace Syrius{

    class GlShader: public Shader{
    public:
        explicit GlShader(const ShaderDesc& desc);

        ~GlShader() override;

        void bind() override;

        [[nodiscard]] u64 getIdentifier() const override;

    private:
        u32 m_ShaderID;

    };

}

