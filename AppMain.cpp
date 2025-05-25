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
#include "SyriusCoreApp/Layers/ShaderLayer.hpp"
#include "SyriusCoreApp/Layers/CubeMapLayer.hpp"
#include "SyriusCoreApp/Layers/DeferredPBRLayer.hpp"

template<typename T>
void runTest(const std::string& iniFile, const std::string& windowTitle){
    try {
        SyriusCoreDev core(iniFile, windowTitle);
        core.pushLayer<T>();
        core.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }
}

void runAllTests(const std::string& iniFile){
    runTest<ImGuiLayer>(iniFile, "ImGui");
    runTest<VertexBufferLayer>(iniFile, "VertexBuffer");
    runTest<IndexBufferLayer>(iniFile, "IndexBuffer");
    runTest<VertexArrayLayer>(iniFile, "VertexArray");
    runTest<ShaderLayer>(iniFile, "Shader");
    runTest<ConstantBufferLayer>(iniFile, "ConstantBuffer");
    runTest<SamplerLayer>(iniFile, "Sampler");
    runTest<TextureLayer>(iniFile, "Texture");
    runTest<DepthTestLayer>(iniFile, "DepthTest");
    runTest<TextureAtlasLayer>(iniFile, "TextureAtlas");
    runTest<FrameBufferLayer>(iniFile, "FrameBuffer");
    runTest<CubeMapLayer>(iniFile, "CubeMap");
    runTest<DeferredPBRLayer>(iniFile, "DeferredPBR");
}

int main(int argc, char** argv) {
    try {
        std::string configFile = "Config.ini";
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
        }
        else{
            configFile = argv[1];
        }
        if (argc == 2){
            runAllTests(configFile);
        }
        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "ImGui") {
                runTest<ImGuiLayer>(configFile, arg);
            } else if (arg == "VertexBuffer") {
                runTest<VertexBufferLayer>(configFile, arg);
            } else if (arg == "IndexBuffer") {
                runTest<IndexBufferLayer>(configFile, arg);
            } else if (arg == "VertexArray") {
                runTest<VertexArrayLayer>(configFile, arg);
            } else if (arg == "Shader") {
                runTest<ShaderLayer>(configFile, arg);
            } else if (arg == "ConstantBuffer") {
                runTest<ConstantBufferLayer>(configFile, arg);
            } else if (arg == "Sampler") {
                runTest<SamplerLayer>(configFile, arg);
            } else if (arg == "Texture") {
                runTest<TextureLayer>(configFile, arg);
            } else if (arg == "DepthTest") {
                runTest<DepthTestLayer>(configFile, arg);
            } else if (arg == "TextureAtlas") {
                runTest<TextureAtlasLayer>(configFile, arg);
            } else if (arg == "FrameBuffer") {
                runTest<FrameBufferLayer>(configFile, arg);
            } else if (arg == "CubeMap") {
                runTest<CubeMapLayer>(configFile, arg);
            } else if (arg == "All") {
                runAllTests(configFile);
            } else if (arg == "DeferredPBR") {
                runTest<DeferredPBRLayer>(configFile, arg);
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

    printf("Allocated Memory: %zu\n", MemoryProfiler::getAllocatedMemory());
    printf("Freed Memory: %zu\n", MemoryProfiler::getDeallocatedMemory());
    printf("Memory Usage: %zu\n", MemoryProfiler::getCurrentUsage());

    return 0;
}
