#include "Utils.hpp"

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
    deviceInfo += "Max Constant Buffer Slots        : " + std::to_string(deviceLimits->getMaxConstantBufferSlots()) + "\n";
    deviceInfo += "Max Constant Buffer Size         : " + std::to_string(deviceLimits->getMaxConstantBufferSize()) + "\n";
    deviceInfo += "Max Shader Storage Buffer Size   : " + std::to_string(deviceLimits->getMaxShaderStorageBufferSize()) + "\n";
    deviceInfo += "Max Shader Storage Buffer Slots  : " + std::to_string(deviceLimits->getMaxShaderStorageBufferSlots()) + "\n";
    deviceInfo += "Max Depth Buffer Bits            : " + std::to_string(deviceLimits->getMaxDepthBufferBits()) + "\n";
    deviceInfo += "Max Vertex Attributes            : " + std::to_string(deviceLimits->getMaxVertexAttributes()) + "\n";
    deviceInfo += "Max Vertex Count                 : " + std::to_string(deviceLimits->getMaxVertexCount()) + "\n";
    deviceInfo += "Max Index Count                  : " + std::to_string(deviceLimits->getMaxIndexCount()) + "\n";
    deviceInfo += "Largest Possible Index           : " + std::to_string(deviceLimits->getMaxIndex()) + "\n";
    deviceInfo += "Is Double Buffered               : " + std::to_string(deviceLimits->isDoubleBuffered()) + "\n";

    std::cout << deviceInfo << std::endl;
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

ShaderProgram loadShader(const ResourceView<Context>& context, const std::string& vertexShader, const std::string& fragmentShader) {
    fs::path library = fs::current_path() / "Resources" / "Shaders";
    std::string extension;
    SR_SHADER_LANGUAGE_TYPE codeType;
    switch (context->getType()){
        case SR_API_OPENGL: {
            library /= "GLSL";
            extension = ".glsl";
            codeType = SR_SHADER_LANGUAGE_GLSL;
            break;
        }
        case SR_API_D3D11:{
            library /= "HLSL";
            extension = ".hlsl";
            codeType = SR_SHADER_LANGUAGE_HLSL;
            break;
        }
        default:
            throw std::runtime_error("ShaderLibrary::loadShader: Unsupported API");
    }

    ShaderProgram program;

    ShaderModuleFileDesc vsDesc;
    vsDesc.shaderType = SR_SHADER_VERTEX;
    vsDesc.language = codeType;
    vsDesc.filePath = library / std::string(vertexShader + "_vs" + extension);
    vsDesc.entryPoint = "main";
    program.vertexShader = context->createShaderModule(vsDesc);

    ShaderModuleFileDesc fsDesc;
    fsDesc.shaderType = SR_SHADER_FRAGMENT;
    fsDesc.language = codeType;
    fsDesc.filePath = library / std::string(fragmentShader + "_fs" + extension);
    fsDesc.entryPoint = "main";
    program.fragmentShader = context->createShaderModule(fsDesc);

    ShaderDesc sDesc;
    sDesc.vertexShader = program.vertexShader;
    sDesc.fragmentShader = program.fragmentShader;
    program.shaderProgram = context->createShader(sDesc);

    return program;

}