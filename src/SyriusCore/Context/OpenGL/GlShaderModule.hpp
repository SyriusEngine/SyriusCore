#pragma once

#include "GlUtils.hpp"
#include "../../../../include/SyriusCore/Context/ShaderModule.hpp"

namespace Syrius {

    class GlShaderModule : public ShaderModule {
    public:
        explicit GlShaderModule(const ShaderModuleDesc& desc);

        ~GlShaderModule() override;

        [[nodiscard]] uint64 getIdentifier() const override;

    private:

        void loadSPIRV(const ShaderModuleDesc& desc);

        void loadGlsl(const ShaderModuleDesc& desc);

    private:
        uint32 m_ModuleID;

    };
}


