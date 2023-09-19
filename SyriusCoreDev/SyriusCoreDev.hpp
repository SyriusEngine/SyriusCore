#pragma once

#include "Include.hpp"

class SyriusCoreDev{
public:
    explicit SyriusCoreDev(const std::string& iniFile);

    ~SyriusCoreDev();

    void run();

private:
    EasyIni::Configuration m_Config;
    Resource<SyriusWindow> m_Window;
    ResourceView<Context> m_Context;
};