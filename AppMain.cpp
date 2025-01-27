#include "SyriusCoreApp/SyriusCoreApp.hpp"

#include "SyriusCoreApp/Components/DebugInfo.hpp"

void runBasic(const std::string& configFile = "./Config.ini"){
    SyriusCoreApp app(configFile);
    app.addComponent<DebugInfo>();
    app.run();
}



int main(int argc, char** argv) {
    try {
        std::string configFile = "./Config.ini";
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
            configFile = argv[1];
        }
        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "Basic") {
                runBasic(configFile);
            } else {
                std::cerr << "Unknown test: " << arg << std::endl;
            }
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }

    return 0;
}
