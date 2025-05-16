#pragma once

#include "SandBoxInclude.hpp"

class Sandbox {
public:
    Sandbox(const std::string& iniFile, const std::vector<std::string_view>& args);

    ~Sandbox();

    void run();

private:
    EasyIni::Configuration m_Config;

    UP<SyriusWindow> m_Window;
    ResourceView<Context> m_Context;
};