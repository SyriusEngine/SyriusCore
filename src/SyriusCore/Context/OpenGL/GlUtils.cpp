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
        }
    }

    GLenum getGlDrawType(SR_DRAW_TYPE type){
        switch(type){
            case SR_DRAW_POINTS:
                return GL_POINTS;
            case SR_DRAW_LINES:
                return GL_LINES;
            case SR_DRAW_LINES_STRIP:
                return GL_LINE_STRIP;
            case SR_DRAW_TRIANGLES:
                return GL_TRIANGLES;
            case SR_DRAW_TRIANGLE_STRIP:
                return GL_TRIANGLE_STRIP;
        }
    }

    GLenum getGlDataType(SR_DATA_TYPE type){
        switch (type) {
            case SR_FLOAT32:
                return GL_FLOAT;
            case SR_FLOAT64:
                return GL_DOUBLE;
            case SR_INT32:
                return GL_INT;
            case SR_UINT32:
                return GL_UNSIGNED_INT;
            case SR_INT16:
                return GL_SHORT;
            case SR_UINT16:
                return GL_UNSIGNED_SHORT;
            case SR_INT8:
                return GL_BYTE;
            case SR_UINT8:
                return GL_UNSIGNED_BYTE;
            default: {
                SR_CORE_WARNING("Invalid type given to converter, return default: GL_FLOAT");
                return GL_FLOAT;
            }
        }
    }

    GLenum getGlBufferType(SR_BUFFER_TYPE type){
        switch (type){
            case SR_BUFFER_DEFAULT:
                return GL_STATIC_DRAW;
            case SR_BUFFER_DYNAMIC:
                return GL_DYNAMIC_DRAW;
            case SR_BUFFER_STATIC:
                return GL_STATIC_DRAW;
        }
    }
}
