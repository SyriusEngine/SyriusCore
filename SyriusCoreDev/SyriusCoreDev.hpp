#pragma once

#include "Utils.hpp"

class SyriusCoreDev{
public:
    explicit SyriusCoreDev(const std::string& iniFile);

    ~SyriusCoreDev();

    void run();

private:

    void processEvents();

private:
    EasyIni::Configuration m_Config;
    Resource<SyriusWindow> m_Window;
    ResourceView<Context> m_Context;
};