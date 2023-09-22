#include "SyriusCoreDev/SyriusCoreDev.hpp"
#include "AppLayer.hpp"
#include "TestLayers/BasicLayer.hpp"


int main(int argc, char** argv) {
    try {
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

    return 0;
}