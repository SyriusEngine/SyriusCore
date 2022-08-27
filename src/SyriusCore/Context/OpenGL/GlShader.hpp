#pragma once

#include "GlUtils.hpp"
#include "GlShaderModule.hpp"
#include "../../../../include/SyriusCore/Context/Shader.hpp"

namespace Syrius{

    class GlShader: public Shader{
    public:
        GlShader();

        ~GlShader() override;

        void addShaderModule(ShaderModule* module) override;

        void link() override;

        void bind() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:
        uint32 m_ShaderID;
        std::unordered_map<std::string, int32> m_Uniforms;

    };

}

