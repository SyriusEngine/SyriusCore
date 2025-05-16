#include <iostream>

#include <SyriusUtils/Logger/Logger.hpp>

int main(const int argc, char* argv[]) {
    try {
        std::vector<std::string_view> args(argv, argv + argc);

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (Syrius::SyriusAssert& assert) {
        std::cerr << "========================SYRIUS ASSERT TRIGGERED========================\n\n";
        std::cerr << assert.what() << std::endl;
        return 2;
    }
    return 0;
}
