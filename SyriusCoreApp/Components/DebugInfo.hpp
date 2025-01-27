#pragma once

#include "../AppComponent.hpp"

class DebugInfo : public AppComponent{
public:
    explicit DebugInfo(UP<SyriusWindow>& window, ResourceView<Context>& context);

    ~DebugInfo() override = default;

    void onImGuiRender(ImGuiSizeData& sizeData) override;
};