#include <iostream>
#include "SyriusCoreSandBox/SandBox.hpp"

int main(const int argc, char* argv[]) {
    try {
        std::string configFile = "config.ini";
        if (argc > 1) {
            configFile = argv[1];
        }
        const std::vector<std::string_view> args(argv, argv + argc);
        Sandbox sandbox(configFile, args);
        sandbox.run();

    } catch (SyriusAssert& assert) {
        std::cerr << "========================SYRIUS ASSERT TRIGGERED========================\n\n";
        std::cerr << assert.what() << std::endl;
        return 2;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
