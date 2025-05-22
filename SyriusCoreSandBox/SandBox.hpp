#pragma once

#include "SandBoxInclude.hpp"
#include "Core/ComponentContainer.hpp"

class Sandbox {
public:
    Sandbox(const std::string& iniFile, const std::vector<std::string_view>& args);

    ~Sandbox();

    void run();


private:

    void processEvents() const;

    void setupWindow();

    void setupContext();

    void setupImGui();

private:
    EasyIni::Configuration m_Config;

    UP<SyriusWindow> m_Window;
    ResourceView<Context> m_Context;

    UP<ComponentContainer> m_ComponentContainer;

    bool m_PrintEventInfo = false;
};
