#include "GlUtils.hpp"

namespace Syrius{

    GLenum getGlShaderType(SR_SHADER_TYPE type){
        switch (type){
            case SR_SHADER_VERTEX:
                return GL_VERTEX_SHADER;
            case SR_SHADER_FRAGMENT:
                return GL_FRAGMENT_SHADER;
            case SR_SHADER_GEOMETRY:
                return GL_GEOMETRY_SHADER;
            case SR_SHADER_TESSELATION:
                return GL_TESS_EVALUATION_SHADER;
            case SR_SHADER_COMPUTE:
                return GL_COMPUTE_SHADER;
            default:
                return 0;
        }
    }
}
