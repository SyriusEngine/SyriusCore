#pragma once

#include "Utils.hpp"
#include "Layer.hpp"
#include <chrono>

class SyriusCoreDev{
public:
    explicit SyriusCoreDev(const std::string& iniFile, const std::string& windowTitle = "SyriusCoreDev");

    ~SyriusCoreDev();

    void run();

    template<typename T, typename... Args>
    void pushLayer(Args&&... args){
        m_Layers.emplace_back(std::make_shared<T>(m_Context, m_Window, m_Config, std::forward<Args>(args)...));
    }


private:

    void processEvents();

private:
    EasyIni::Configuration m_Config;
    Resource<SyriusWindow> m_Window;
    ResourceView<Context> m_Context;

    std::vector<RCP<Layer>> m_Layers;
};