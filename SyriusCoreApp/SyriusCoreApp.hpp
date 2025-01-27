#pragma once

#include "Utils.hpp"
#include "AppComponent.hpp"

class SyriusCoreApp{
public:
    explicit SyriusCoreApp(const std::string& configFile = "Config.ini", const std::string& windowTitle = "SyriusCoreApp");

    ~SyriusCoreApp();

    void run();

    template<typename T, typename... Args>
    void addComponent(Args&&... args){
        m_Components.emplace_back(std::make_unique<T>(m_Window, m_Context, std::forward<Args>(args)...));
    }

private:

    void processEvents();

private:
    bool m_EnableImGui = true;
    EasyIni::Configuration m_Config;
    UP<SyriusWindow> m_Window;
    ResourceView<Context> m_Context;

    std::vector<UP<AppComponent>> m_Components;
};


