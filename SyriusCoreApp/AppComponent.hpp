#pragma once

#include "Utils.hpp"

struct ImGuiSizeData{
    float nextX = 0.0f;
    float nextY = 0.0f;
    float nextWidth = 200.0f;
    float nextHeight = 0.0f;
};

struct Drawable{
    ResourceView<VertexArray> vertexArray;
    ResourceView<VertexBuffer> vertexBuffer;
    ResourceView<IndexBuffer> indexBuffer;
};

class AppComponent{
public:
    AppComponent() = delete;

    virtual ~AppComponent() = default;

    virtual void onEvent(Event& event);

    virtual void onRender();

    virtual void onImGuiRender(ImGuiSizeData& sizeData);

protected:
    AppComponent(UP<SyriusWindow>& window, ResourceView<Context>& context);

    Drawable loadMesh(Mesh &mesh, ShaderProgram &program, SR_BUFFER_USAGE usage = SR_BUFFER_USAGE_DEFAULT);

    ShaderProgram loadShader(const std::string& vertexShader, const std::string& fragmentShader = "");

    void imGuiBeginPanel(ImGuiSizeData& sizeData, const char* name) const;

    void imGuiEndPanel(ImGuiSizeData& sizeData);

protected:
    UP<SyriusWindow>& m_Window;
    ResourceView<Context>& m_Context;
};