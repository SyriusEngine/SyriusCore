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

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_ShaderID;

    };

}

