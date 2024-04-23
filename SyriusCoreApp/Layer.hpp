#pragma once

#include <functional>
#include "Utils.hpp"
#include "ShaderLibrary.hpp"

using DrawFunction = std::function<void()>;

struct TextureParameters{
    float lerpFactor = 0.5f;
    float scale = 1.0f;
    float padding[2] = {0.0f, 0.0f};
};

class Layer{
public:
    Layer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    virtual ~Layer();

    virtual void onUpdate() = 0;

    virtual void onEvent(const Event& event) = 0;

protected:

    ResourceView<VertexArray> loadMesh(Mesh& mesh, ShaderProgram& program);

    void renderImGui();

    void addImGuiDrawFunction(DrawFunction drawFunction);

    void imGuiDebugPanel(ResourceView<Context>&);

    void imGuiIndexBufferPanel(ResourceView<IndexBuffer>&);

    void imGuiVertexArrayPanel(ResourceView<VertexArray>&);

    void imGuiRenderTransformConstantBuffer(ResourceView<ConstantBuffer>& );

    void imGuiSamplerPanel(ResourceView<Sampler>&);

    void imGuiTextureParametersPanel(ResourceView<ConstantBuffer>& constantBuffer);

    void imGuiTexturePanel(ResourceView<Texture2D>& texture);

protected:
    ResourceView<Context>& m_Context;
    EasyIni::Configuration& m_Config;
    const Resource<SyriusWindow>& m_Window;
    ShaderLibrary m_ShaderLibrary;

private:
    std::vector<DrawFunction> m_ImGuiDraw;

};
