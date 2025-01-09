#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/ShaderModule.hpp"

namespace Syrius {

    class GlShaderModule : public ShaderModule {
    public:
        explicit GlShaderModule(const ShaderModuleDesc& desc);

        explicit GlShaderModule(const ShaderModuleFileDesc& desc);

        ~GlShaderModule() override;

        [[nodiscard]] u64 getIdentifier() const override;

    private:

        void loadSPIRV(const std::string& code);

        void loadGlsl(const std::string& code);

    private:
        u32 m_ModuleID;

    };
}


