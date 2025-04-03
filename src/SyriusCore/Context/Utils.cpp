#include "../../../include/SyriusCore/Context/Utils.hpp"
#include "../Utils/DebugMacros.hpp"

namespace Syrius{

    std::string getShaderTypeString(SR_SHADER_TYPE type){
        switch (type) {
            case SR_SHADER_VERTEX:                  return "Vertex Shader";
            case SR_SHADER_FRAGMENT:                return "Fragment Shader";
            case SR_SHADER_GEOMETRY:                return "Geometry Shader";
            case SR_SHADER_TESSELATION_EVALUATION:  return "Tessellation Evaluation Shader";
            case SR_SHADER_TESSELATION_CONTROL:     return "Tessellation Control Shader";
            case SR_SHADER_COMPUTE:                 return "Compute Shader";
            default: {
                SR_LOG_WARNING("Utils", "Unknown shader type {} given to converter", type);
                return "";
            }
        }
    }
}