#include "SyriusCoreDev/SyriusCoreDev.hpp"
#include "AppLayer.hpp"
#include "TestLayers/BasicLayer.hpp"

void test(){
    ImageFileDesc desc;
    desc.fileName = "./Resources/Textures/Texture1.jpg";
    auto image = Syrius::createImage(desc);
    printf("Image: %d %d\n", image->getWidth(), image->getHeight());
    printf("ChannelCount: %d\n", image->getChannelCount());

//    image->extendAlpha();
    desc.fileName = "./Resources/Textures/Texture2.jpg";
    desc.imgType = SR_IMAGE_JPG;
    image->writeToFile(desc);

}


int main(int argc, char** argv) {
    try {
        test();
        return 0;
        for (int i = 1; i < argc; ++i) {
            SyriusCoreDev dev(argv[i]);
            dev.pushLayer<AppLayer>();
            dev.run();
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