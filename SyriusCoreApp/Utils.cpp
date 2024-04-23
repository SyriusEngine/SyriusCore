#include "Utils.hpp"

Mesh createTriangle(){
    Mesh triangle;
    triangle.vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.0f, 0.5f, 0.0f},   {0.0f, 1.0f, 0.0f}, {0.5f, 1.0f}},
            {{0.5f, -0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}
    };
    triangle.indices = {
            0, 1, 2
    };
    return triangle;
}

Mesh createRectangle(){
    Mesh mesh;
    mesh.vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f},   {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
    };
    mesh.indices = {
            0, 3, 1,
            3, 2, 1
    };
    return mesh;
}

#if defined(SR_COMPILER_MSVC)
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#endif

Mesh createSphere(uint32 rings, uint32 sectors){
    Mesh mesh;
    float const R = 1.0f/(float)(rings-1);
    float const S = 1.0f/(float)(sectors-1);
    uint32 r, s;

    for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            mesh.vertices.push_back({{x, y, z},  {x, y, z}, {s*S, r*R}});
        }

    for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
            mesh.indices.push_back(r * sectors + s);
            mesh.indices.push_back(r * sectors + (s+1));
            mesh.indices.push_back((r+1) * sectors + (s+1));

            mesh.indices.push_back(r * sectors + s);
            mesh.indices.push_back((r+1) * sectors + (s+1));
            mesh.indices.push_back((r+1) * sectors + s);
        }
    return mesh;
}

Mesh createCube(){
    Mesh cube;
    cube.vertices = {
            // front
            {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            // left
            {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            // bottom
            {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            // back
            {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            // right
            {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            // top
            {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}

    };
    cube.indices = {
            0, 3, 2,
            0, 2, 1,

            4, 7, 6,
            4, 6, 5,

            8, 11, 10,
            8, 10, 9,

            12, 15, 14,
            12, 14, 13,

            16, 19, 18,
            16, 18, 17,

            20, 23, 22,
            20, 22, 21
    };
    return cube;
}

void printEventInfo(const Event& event){
    switch (event.type){
        case SR_EVENT_WINDOW_CLOSED:
            std::cout << "Window closed event" << std::endl;
            break;
        case SR_EVENT_WINDOW_RESIZED:
            std::cout << "Window resized event: " << event.windowWidth << "x" << event.windowHeight << std::endl;
            break;
        case SR_EVENT_WINDOW_MOVED:
            std::cout << "Window moved event: x: " << event.windowPosX << " y:" << event.windowPosY << std::endl;
            break;
        case SR_EVENT_WINDOW_GAINED_FOCUS:
            std::cout << "Window focus gained event" << std::endl;
            break;
        case SR_EVENT_WINDOW_LOST_FOCUS:
            std::cout << "Window focus lost event" << std::endl;
            break;
        case SR_EVENT_WINDOW_FILE_DROPPED:
            std::cout << "Window file dropped event: " << event.droppedFilePath << std::endl;
            break;
        case SR_EVENT_MOUSE_ENTERED:
            std::cout << "Window mouse entered event" << std::endl;
            break;
        case SR_EVENT_MOUSE_LEFT:
            std::cout << "Window mouse left event" << std::endl;
            break;
        case SR_EVENT_MOUSE_MOVED:
            std::cout << "Window mouse moved event: x: " << event.mousePosX << " y:" << event.mousePosY << std::endl;
            break;
        case SR_EVENT_MOUSE_SCROLLED:
            std::cout << "Window mouse scrolled event: X: " << event.mouseScrollX << " y:" << event.mouseScrollY << std::endl;
            break;
        case SR_EVENT_MOUSE_BUTTON_PRESSED:
            std::cout << "Window mouse button pressed event: " << event.mouseButton << std::endl;
            break;
        case SR_EVENT_MOUSE_BUTTON_RELEASED:
            std::cout << "Window mouse button released event: " << event.mouseButton << std::endl;
            break;
        case SR_EVENT_KEYBOARD_KEY_PRESSED:
            std::cout << "Window keyboard key pressed event: " << event.keyCode << std::endl;
            break;
        case SR_EVENT_KEYBOARD_KEY_RELEASED:
            std::cout << "Window keyboard key released event: " << event.keyCode << std::endl;
            break;
        case SR_EVENT_KEYBOARD_KEY_TYPED:
            std::cout << "Window keyboard key typed event: " << event.keyCode << std::endl;
            break;
        case SR_EVENT_RAW_MOUSE_MOVED:
            std::cout << "Window raw mouse moved event: x: " << event.mousePosX << " y:" << event.mousePosY << std::endl;
            break;
        case SR_EVENT_RAW_KEYBOARD_KEY_PRESSED:
            std::cout << "Window raw keyboard key pressed event: " << event.keyCode << std::endl;
            break;
        case SR_EVENT_RAW_KEYBOARD_KEY_RELEASED:
            std::cout << "Window raw keyboard key released event: " << event.keyCode << std::endl;
            break;
        default:
            std::cout << "Unknown event" << std::endl;
            break;
    }
}

void printContextInfo(const ResourceView<Context>& context){
    const auto& deviceLimits = context->getDeviceLimits();
    std::string deviceInfo = "==============Device Info======================\n";
    deviceInfo += "API Version                      : " + deviceLimits->getAPIVersion() + "\n";
    deviceInfo += "Device name                      : " + deviceLimits->getDeviceName() + "\n";
    deviceInfo += "Device Vendor                    : " + deviceLimits->getDeviceVendor() + "\n";
    deviceInfo += "Shading Language Version         : " + deviceLimits->getShadingLanguageVersion() + "\n";
    deviceInfo += "Max FrameBuffer Width            : " + std::to_string(deviceLimits->getMaxFramebufferWidth()) + "\n";
    deviceInfo += "Max FrameBuffer Height           : " + std::to_string(deviceLimits->getMaxFramebufferHeight()) + "\n";
    deviceInfo += "Max FrameBuffer Color Attachments: " + std::to_string(deviceLimits->getMaxFramebufferColorAttachments()) + "\n";
    deviceInfo += "Max Viewport Width               : " + std::to_string(deviceLimits->getMaxViewportWidth()) + "\n";
    deviceInfo += "Max Viewport Height              : " + std::to_string(deviceLimits->getMaxViewportHeight()) + "\n";
    deviceInfo += "Max Texture Slots                : " + std::to_string(deviceLimits->getMaxTextureSlots()) + "\n";
    deviceInfo += "Max Texture Size                 : " + std::to_string(deviceLimits->getMaxTextureSize()) + "\n";
    deviceInfo += "Max SSBO Slots                   : " + std::to_string(deviceLimits->getMaxSSSBOSlots()) + "\n";
    deviceInfo += "Max Constant Buffer Slots        : " + std::to_string(deviceLimits->getMaxConstantBufferSlots()) + "\n";
    deviceInfo += "Max Constant Buffer Size         : " + std::to_string(deviceLimits->getMaxConstantBufferSize()) + "\n";
    deviceInfo += "Max Depth Buffer Bits            : " + std::to_string(deviceLimits->getMaxDepthBufferBits()) + "\n";
    deviceInfo += "Max Vertex Attributes            : " + std::to_string(deviceLimits->getMaxVertexAttributes()) + "\n";
    deviceInfo += "Max Vertex Count                 : " + std::to_string(deviceLimits->getMaxVertexCount()) + "\n";
    deviceInfo += "Max Index Count                  : " + std::to_string(deviceLimits->getMaxIndexCount()) + "\n";
    deviceInfo += "Largest Possible Index           : " + std::to_string(deviceLimits->getMaxIndex()) + "\n";
    deviceInfo += "Is Double Buffered               : " + std::to_string(deviceLimits->isDoubleBuffered()) + "\n";

    std::cout << deviceInfo << std::endl;
}

std::string drawModeToString(SR_DRAW_MODE mode){
    switch (mode){
        case SR_DRAW_POINTS:        return "Points";
        case SR_DRAW_LINES:         return "Lines";
        case SR_DRAW_LINES_STRIP:   return "Line Strip";
        case SR_DRAW_TRIANGLES:     return "Triangles";
        case SR_DRAW_TRIANGLE_STRIP:return "Triangle Strip";
        default:
            return "Unknown";
    }
}

std::string filterToString(SR_TEXTURE_FILTER mode){
    switch (mode){
        case SR_TEXTURE_FILTER_NONE:    return "None";
        case SR_TEXTURE_FILTER_LINEAR:  return "Linear";
        case SR_TEXTURE_FILTER_POINT:   return "Point";
        default:
            return "Unknown";
    }
}

std::string wrapModeToString(SR_TEXTURE_WRAP mode){
    switch (mode){
        case SR_TEXTURE_WRAP_NONE:          return "None";
        case SR_TEXTURE_WRAP_REPEAT:        return "Repeat";
        case SR_TEXTURE_WRAP_MIRROR_REPEAT: return "Mirror Repeat";
        case SR_TEXTURE_WRAP_CLAMP_EDGE:    return "Clamp Edge";
        case SR_TEXTURE_WRAP_CLAMP_BORDER:  return "Clamp Border";
        default:
            return "Unknown";
    }
}
