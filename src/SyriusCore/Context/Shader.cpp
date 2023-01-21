#include "../../../../include/SyriusCore/Context/Shader.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    Shader::Shader(const ShaderDesc& desc){
        SR_CORE_PRECONDITION(desc.vertexShader.isValid(), "Vertex shader is null");
        SR_CORE_PRECONDITION(desc.fragmentShader.isValid(), "Fragment shader is null");
    }

    Shader::~Shader() = default;
}