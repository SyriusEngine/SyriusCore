#include "Tests/TestEnvironment.hpp"
#include <fstream>

class TestDebugMessageHandler{
public:
    static void init(const std::string& debugOutput){
        m_File = std::ofstream(debugOutput);
 //       setDebugMessageCallback(TestDebugMessageHandler::messageCallback);
    }

    static void terminate(){
        m_File.close();
    }

    static void messageCallback(const Message& msg){
        std::string severity;
        switch (msg.severity){
            case SR_CORE_MESSAGE_SEVERITY_INFO:     severity = "INFO";  break;
            case SR_CORE_MESSAGE_SEVERITY_LOW:      severity = "LOW";   break;
            case SR_CORE_MESSAGE_SEVERITY_MEDIUM:   severity = "MEDIUM";break;
            case SR_CORE_MESSAGE_SEVERITY_HIGH:     severity = "HIGH";  break;
        }

        std::string source;
        switch (msg.messageType){
            case SR_CORE_MESSAGE: source = "MESSAGE"; break;
            case SR_CORE_MESSAGE_OPENGL: source = "OPENGL"; break;
            case SR_CORE_MESSAGE_VULKAN: source = "VULKAN"; break;
            case SR_CORE_MESSAGE_D3D11: source = "D3D11"; break;
            case SR_CORE_MESSAGE_HRESULT: source = "HRESULT"; break;
            case SR_CORE_MESSAGE_DXGI: source = "DXGI"; break;
            case SR_CORE_MESSAGE_WIN32: source = "WIN32"; break;
            case SR_CORE_MESSAGE_X11: source = "X11"; break;
            case SR_CORE_MESSAGE_PRECONDITION: source = "PRECONDITION"; break;
            case SR_CORE_MESSAGE_POSTCONDITION: source = "POSTCONDITION"; break;
            case SR_CORE_MESSAGE_ASSERTION: source = "ASSERTION"; break;
        }
        auto message = msg.message;
        message.erase(remove(message.begin(), message.end(), '\n'), message.end());
        std::string final = "[" + msg.file + " : " + msg.function + " : " + std::to_string(msg.line) + "][" + source + " : " + severity + "]: " + message + "\n";

        m_File << final;
    }

private:
    static std::ofstream m_File;
};

std::ofstream TestDebugMessageHandler::m_File;

int32 runTests(const EnvironmentDesc& envDesc){
    // testing::internal::CaptureStdout();
    TestEnvironment::setup(envDesc);
    auto retVal = RUN_ALL_TESTS();
    TestEnvironment::teardown();
//    std::string output = testing::internal::GetCapturedStdout();
//    std::cout << output << std::endl;
    return retVal;
}

int main(int argc, char** argv) {
    TestDebugMessageHandler::init("TestMessageOutput.txt");
    testing::InitGoogleTest(&argc, argv);

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <Option (OpenGL/D3D11)>" << std::endl;
        return 1;
    }
    int32 retVal = 0;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "OpenGL"){
            std::cerr << "\n\n================ Running OpenGL Tests ================\n" << std::endl;
            EnvironmentDesc glDesc;
            glDesc.api = SR_API_OPENGL;
            glDesc.exportImages = true;
            glDesc.title = "SyriusCore GTest (OpenGL)";
            retVal += runTests(glDesc);
        }
        else if (arg == "D3D11"){
            std::cerr << "\n\n================= Running D3D11 Tests ================\n" << std::endl;
            EnvironmentDesc d3d11Desc;
            d3d11Desc.api = SR_API_D3D11;
            d3d11Desc.exportImages = true;
            d3d11Desc.title = "SyriusCore GTest (D3D11)";
            retVal += runTests(d3d11Desc);
        }
        else{
            std::cerr << "Unknown Arg: " << arg << std::endl;
        }
    }
    return retVal;
}

