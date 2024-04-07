#include "SyriusCoreDev/SyriusCoreDev.hpp"
#include "AppLayers/TestContext.hpp"
#include "AppLayers/TestImGui.hpp"
#include "AppLayers/TestVertexBuffer.hpp"
#include "AppLayers/TestIndexBuffer.hpp"
#include "AppLayers/TestTexture.hpp"
#include "AppLayers/TestConstantBuffer.hpp"
#include "AppLayers/TestDepthTest.hpp"
#include "AppLayers/TestFrameBuffer.hpp"
#include "AppLayers/TestCubemap.hpp"

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
    runTest<TestCubemap>(iniFile);
}

int experiment(){
    WindowDesc wDesc;
    auto window = createWindow(wDesc);
    ContextDesc cDesc;
    cDesc.api = SR_API_D3D11;
    auto context = window->createContext(cDesc);

    ImageFileDesc iDesc;
    iDesc.fileName = "./Resources/Textures/insta.png";
    iDesc.flipOnAccess = true;
    auto img = createImage(iDesc);
    Texture2DImageDesc tDesc;
    tDesc.image = createResourceView(img);
    auto texture = context->createTexture2D(tDesc);

    // get Data
    auto img2 = texture->getData();
    ImageFileDesc iDesc2;
    iDesc2.fileName = "./output.png";
    img2->writeToFile(iDesc2);

    // create a cpp file containing a static vector of the image data
    std::ofstream file("./Tests/Inst48a512.hpp");
    file << "#include <vector>\n";
    file << "#include <cstdint>\n";
    file << "static uint32_t s_InstaWidth = " << img2->getWidth() << ";\n";
    file << "static uint32_t s_InstaHeight = " << img2->getHeight() << ";\n";
    file << "static std::vector<uint8_t> s_InstaData = {\n";
    auto ptr = img2->getData();
    auto ptrUint8 = reinterpret_cast<const uint8_t*>(ptr);
    auto size = img2->getWidth() * img2->getHeight() * img2->getChannelCount();
    for (uint32_t i = 0; i < size; i++) {
        file << static_cast<int>(ptrUint8[i]);
        if (i < size - 1) {
            file << ", ";
        }
    }
    file << "};\n";
    file.close();

    return 0;
}

int main(int argc, char** argv) {
    try {
        // return experiment();
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
            } else if (arg == "Cubemap") {
                runTest<TestCubemap>(argv[1]);
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
