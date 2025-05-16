#pragma once

#include "SandBoxInclude.hpp"

#include "Core/IComponent.hpp"
#include "Core/IComponentFactory.hpp"

class Sandbox {
public:
    Sandbox(const std::string& iniFile, const std::vector<std::string_view>& args);

    ~Sandbox();

    void run();

    template<typename FACTORY>
    void addFactory() {
        static_assert(std::is_base_of_v<IComponentFactory, FACTORY>, "FACTORY type is not derived from IFactoryComponent");

        auto factory = createSP<FACTORY>();
        m_ComponentFactories.insert({factory->provides(), factory});
    }

private:

    void processEvents();

    void setupWindow();

    void setupContext();

    void setupImGui();

private:
    EasyIni::Configuration m_Config;

    UP<SyriusWindow> m_Window;
    ResourceView<Context> m_Context;

    bool m_PrintEventInfo = false;

    std::vector<SP<IComponent>> m_Components;
    std::unordered_map<std::string, SP<IComponentFactory>> m_ComponentFactories;
};
