#include "SyriusCoreDev/SyriusCoreDev.hpp"
#include "AppLayer.hpp"

int main(int argc, char** argv) {
    try {
        ImageFileDesc imgHdrDesc;
        imgHdrDesc.fileName = "./Resources/Textures/photo_studio_loft_hall_4k.hdr";
        imgHdrDesc.flipOnAccess = true;
        auto imgHdr = createImage(imgHdrDesc);
        imgHdr->extendAlpha();

        ImageFileDesc imgDesc;
        imgDesc.fileName = "./Resources/Textures/photo_studio_loft_hall_4k-4channels.hdr";
        imgDesc.flipOnAccess = true;
        imgDesc.imgType = SR_IMAGE_HDR;
        imgHdr->writeToFile(imgDesc);

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