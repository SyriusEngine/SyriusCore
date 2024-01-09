#include "SyriusCoreDev/SyriusCoreDev.hpp"
#include "AppLayer.hpp"

void testImage(){
    ImageFileDesc desc;
    desc.fileName = "./Resources/Textures/HDR_040_Field/HDR_040_Field.hdr";
    auto img = createImage(desc);

    img->extendAlpha();

    ImageFileDesc writeDesc;
    writeDesc.fileName = "./Resources/Textures/HDR_040_Field/HDR_040_Field__2.hdr";
    writeDesc.imgType = SR_IMAGE_HDR;
    img->writeToFile(writeDesc);
}

int main(int argc, char** argv) {
    try {
        testImage();
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