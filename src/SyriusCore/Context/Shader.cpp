#include "../../../../include/SyriusCore/Context/Shader.hpp"
#include "../Core/DebugMacros.hpp"

namespace Syrius{

    Shader::Shader(const ShaderDesc& desc){
        SR_CORE_PRECONDITION(desc.m_VertexShader.isValid(), "Vertex shader is null");
        SR_CORE_PRECONDITION(desc.m_FragmentShader.isValid(), "Fragment shader is null");
    }

    Shader::~Shader() = default;
}