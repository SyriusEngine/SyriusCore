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
#include "SyriusCoreApp/Layers/PhongLightLayer.hpp"
#include "SyriusCoreApp/Layers/ShaderLayer.hpp"
#include "SyriusCoreApp/Layers/CubeMapLayer.hpp"

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
    runTest<PhongLightLayer>(iniFile, "PhongLight");
    runTest<CubeMapLayer>(iniFile, "CubeMap");

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
                runTest<ImGuiLayer>(argv[1], arg);
            } else if (arg == "VertexBuffer") {
                runTest<VertexBufferLayer>(argv[1], arg);
            } else if (arg == "IndexBuffer") {
                runTest<IndexBufferLayer>(argv[1], arg);
            } else if (arg == "VertexArray") {
                runTest<VertexArrayLayer>(argv[1], arg);
            } else if (arg == "Shader") {
                runTest<ShaderLayer>(argv[1], arg);
            } else if (arg == "ConstantBuffer") {
                runTest<ConstantBufferLayer>(argv[1], arg);
            } else if (arg == "Sampler") {
                runTest<SamplerLayer>(argv[1], arg);
            } else if (arg == "Texture") {
                runTest<TextureLayer>(argv[1], arg);
            } else if (arg == "DepthTest") {
                runTest<DepthTestLayer>(argv[1], arg);
            } else if (arg == "TextureAtlas") {
                runTest<TextureAtlasLayer>(argv[1], arg);
            } else if (arg == "FrameBuffer") {
                runTest<FrameBufferLayer>(argv[1], arg);
            } else if (arg == "PhongLight") {
                runTest<PhongLightLayer>(argv[1], arg);
            } else if (arg == "CubeMap") {
                runTest<CubeMapLayer>(argv[1], arg);
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
