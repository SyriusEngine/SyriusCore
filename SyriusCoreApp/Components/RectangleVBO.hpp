#pragma once

#include "../AppComponent.hpp"

class RectangleVBO : public AppComponent{
public:
    explicit RectangleVBO(UP<SyriusWindow>& window, ResourceView<Context>& context);

    ~RectangleVBO() override = default;

    void onRender() override;

    void onImGuiRender(ImGuiSizeData& sizeData) override;

private:
    ResourceView<VertexLayout> m_VertexLayout;
    ResourceView<VertexBuffer> m_Rectangle;
    ResourceView<VertexArray> m_VertexArray;
    ShaderProgram m_ShaderProgram;
};