#pragma once

#include "../AppComponent.hpp"

class RectangleVAO : public AppComponent{
public:
    explicit RectangleVAO(UP<SyriusWindow>& window, ResourceView<Context>& context);

    ~RectangleVAO() override = default;

    void onRender() override;

    void onImGuiRender(ImGuiSizeData& sizeData) override;

private:

    void imGuiRenderIndexBuffer(ImGuiSizeData &sizeData);

    void imGuiRenderVertexArray(ImGuiSizeData &sizeData);

private:
    Drawable m_Rectangle;
    ShaderProgram m_ShaderProgram;
};