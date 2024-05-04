#include "SyriusCoreApp/SyriusCoreDev.hpp"
#include "SyriusCoreApp/Layers/ImGuiLayer.hpp"
#include "SyriusCoreApp/Layers/VertexBufferLayer.hpp"
#include "SyriusCoreApp/Layers/IndexBufferLayer.hpp"
#include "SyriusCoreApp/Layers/VertexArrayLayer.hpp"
#include "SyriusCoreApp/Layers/ConstantBufferLayer.hpp"
#include "SyriusCoreApp/Layers/SamplerLayer.hpp"
#include "SyriusCoreApp/Layers/TextureLayer.hpp"
#include "SyriusCoreApp/Layers/DepthTestLayer.hpp"
#include "SyriusCoreApp/Layers/TextureAtlasLayer.hpp"
#include "SyriusCoreApp/Layers/FrameBufferLayer.hpp"

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
    runTest<ImGuiLayer>(iniFile);
    runTest<VertexBufferLayer>(iniFile);
    runTest<IndexBufferLayer>(iniFile);
    runTest<VertexArrayLayer>(iniFile);
    runTest<ConstantBufferLayer>(iniFile);
    runTest<SamplerLayer>(iniFile);
    runTest<TextureLayer>(iniFile);
    runTest<DepthTestLayer>(iniFile);
    runTest<TextureAtlasLayer>(iniFile);
    runTest<FrameBufferLayer>(iniFile);
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
            if (arg == "ImGui") {
                runTest<ImGuiLayer>(argv[1]);
            } else if (arg == "VertexBuffer") {
                runTest<VertexBufferLayer>(argv[1]);
            } else if (arg == "IndexBuffer") {
                runTest<IndexBufferLayer>(argv[1]);
            }
            else if (arg == "VertexArray") {
                runTest<VertexArrayLayer>(argv[1]);
            }
            else if (arg == "ConstantBuffer") {
                runTest<ConstantBufferLayer>(argv[1]);
            }
            else if (arg == "Sampler") {
                runTest<SamplerLayer>(argv[1]);
            }
            else if (arg == "Texture") {
                runTest<TextureLayer>(argv[1]);
            }
            else if (arg == "DepthTest") {
                runTest<DepthTestLayer>(argv[1]);
            }
            else if (arg == "TextureAtlas") {
                runTest<TextureAtlasLayer>(argv[1]);
            }
            else if (arg == "FrameBuffer") {
                runTest<FrameBufferLayer>(argv[1]);
            }
//            else if (arg == "Cubemap") {
//                runTest<TestCubemap>(argv[1]);
//            }
            else if (arg == "All") {
                runAllTests(argv[1]);
            }
            else {
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
