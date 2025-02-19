#pragma once

#include "../Layer.hpp"

class ImGuiLayer: public Layer{
public:
    ImGuiLayer(ResourceView<Context>& context, const UP<SyriusWindow>& window, EasyIni::Configuration& config);

    ~ImGuiLayer() override;

    void onUpdate() override;

    void onEvent(const Event &event) override;
};