#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"

class TestImGui: public Layer{
public:
    TestImGui(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TestImGui() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;

};