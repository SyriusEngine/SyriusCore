#include "SyriusCoreDev/SyriusCoreDev.hpp"
#include "TestLayers/BasicLayer.hpp"

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

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
            return 1;
        }
        runTest<BasicLayer>(argv[1]);

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
