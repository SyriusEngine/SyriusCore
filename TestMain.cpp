#include "SyriusCoreDev/SyriusCoreDev.hpp"
#include "TestLayers/TestContext.hpp"
#include "TestLayers/TestImGui.hpp"
#include "TestLayers/TestVertexBuffer.hpp"
#include "TestLayers/TestIndexBuffer.hpp"
#include "TestLayers/TestTexture.hpp"
#include "TestLayers/TestConstantBuffer.hpp"
#include "TestLayers/TestDepthTest.hpp"
#include "TestLayers/TestFrameBuffer.hpp"

template<typename T>
void runTest(const std::string& iniFile){
    try {
        SyriusCoreDev core(iniFile);
        core.pushLayer<T>();
        core.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }
}

void runAllTests(const std::string& iniFile){
    runTest<TestContext>(iniFile);
    runTest<TestImGui>(iniFile);
    runTest<TestVertexBuffer>(iniFile);
    runTest<TestIndexBuffer>(iniFile);
    runTest<TestTexture>(iniFile);
    runTest<TestConstantBuffer>(iniFile);
    runTest<TestDepthTest>(iniFile);
    runTest<TestFrameBuffer>(iniFile);
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
            return 1;
        }
        if (argc == 2){
            runAllTests(argv[1]);
        }
        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "Context") {
                runTest<TestContext>(argv[1]);
            } else if (arg == "ImGui") {
                runTest<TestImGui>(argv[1]);
            } else if (arg == "VertexBuffer") {
                runTest<TestVertexBuffer>(argv[1]);
            } else if (arg == "IndexBuffer") {
                runTest<TestIndexBuffer>(argv[1]);
            } else if (arg == "Texture") {
                runTest<TestTexture>(argv[1]);
            } else if (arg == "ConstantBuffer") {
                runTest<TestConstantBuffer>(argv[1]);
            } else if (arg == "DepthTest") {
                runTest<TestDepthTest>(argv[1]);
            } else if (arg == "FrameBuffer") {
                runTest<TestFrameBuffer>(argv[1]);
            } else if (arg == "All") {
                runAllTests(argv[1]);
            } else {
                std::cerr << "Unknown test: " << arg << std::endl;
            }
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }

    printf("Allocated Memory: %zu\n", getAllocatedMemory());
    printf("Freed Memory: %zu\n", getFreedMemory());
    printf("Memory Usage: %zu\n", getMemoryUsage());

    return 0;
}
