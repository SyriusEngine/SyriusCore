#include "../../../include/SyriusCore/Context/Shader.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    Shader::Shader(const ShaderDesc& desc){
        SR_PRECONDITION(desc.vertexShader != nullptr, "Vertex shader is null");
        SR_PRECONDITION(desc.fragmentShader != nullptr, "Fragment shader is null");
    }

    Shader::~Shader() = default;
}