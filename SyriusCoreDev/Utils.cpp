#include "Utils.hpp"

Mesh createRectangle(){
    Mesh mesh;
    mesh.vertices = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f},  {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f},   {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f},  {0.0f, 1.0f}}
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

            mesh.vertices.push_back({{x, y, z},  {s*S, r*R}});
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
            {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

            // left
            {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            // bottom
            {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

            // back
            {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
            {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            // right
            {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
            {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

            // top
            {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
            {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}

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
    std::string deviceInfo = "\n";
    deviceInfo += "Device name                          : " + context->getDeviceName() + "\n";
    deviceInfo += "API Version                          : " + context->getAPIVersion() + "\n";
    deviceInfo += "Shading language version             : " + context->getShadingLanguageVersion() + "\n";
    deviceInfo += "Max Framebuffer width                : " + std::to_string(context->getMaxFramebufferWidth()) + "\n";
    deviceInfo += "Max Framebuffer width                : " + std::to_string(context->getMaxFramebufferHeight()) + "\n";
    deviceInfo += "Max framebuffer texture attachments  : " + std::to_string(context->getMaxFramebufferTextureAttachments()) + "\n";
    deviceInfo += "Max Texture Slots                    : " + std::to_string(context->getMaxTextureSlots()) + " slots\n";
    deviceInfo += "Max Texture2D Size                   : " + std::to_string(context->getMaxTexture2DSize()) + " bytes\n";
    deviceInfo += "Max constant buffer size             : " + std::to_string(context->getMaxConstantBufferSize()) + " bytes\n";
    deviceInfo += "Depth buffer depth                   : " + std::to_string(context->getMaxDepthBufferBits()) + " bits\n";

    std::cout << deviceInfo << std::endl;
}
