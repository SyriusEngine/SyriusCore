#pragma once

#include "../SyriusCoreDev/Layer.hpp"
#include "../SyriusCoreDev/Utils.hpp"

class TestContext: public Layer{
public:
    TestContext(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    ~TestContext() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;
};

