#include "Tests/TestEnvironment.hpp"
#include <fstream>

class TestDebugMessageHandler{
public:
    static void init(const std::string& debugOutput){
        m_File = std::ofstream(debugOutput);
        Logger::setDebugCallback(TestDebugMessageHandler::messageCallback);
    }

    static void terminate(){
        m_File.close();
    }

    static void messageCallback(const Message& message){
        std::string msg = "[Syrius::Logger]";
        msg += " [" + message.file + " : " + message.function + " : " + std::to_string(message.line) + "] ";
        msg += "[" + getMessageSeverityString(message.severity) + "] ";
        msg += "[" + message.source + "]: ";
        msg += message.message + "\n";

        m_File << msg;
    }

private:
    static std::ofstream m_File;
};

std::ofstream TestDebugMessageHandler::m_File;

i32 runTests(const EnvironmentDesc& envDesc){
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
    i32 retVal = 0;
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

