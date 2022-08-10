#pragma once

#include "Utils.hpp"
#include "ShaderModule.hpp"

namespace Syrius{

    class SR_API Shader{
    public:
        Shader();

        virtual ~Shader();

        virtual void addShaderModule(ShaderModule* module) = 0;

        virtual void link() = 0;

        virtual void bind() = 0;

    private:

    };

}



