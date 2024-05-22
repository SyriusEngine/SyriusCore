#pragma once

#include <functional>
#include "Utils.hpp"
#include "ShaderLibrary.hpp"
#include "Camera.hpp"

using DrawFunction = std::function<void()>;

struct TextureParameters{
    float lerpFactor = 0.0f;
    float scale = 1.0f;
    float padding[2] = {0.0f, 0.0f};
};

class Layer{
public:
    Layer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    virtual ~Layer();

    virtual void onUpdate();

    virtual void onEvent(const Event& event);

protected:

    ResourceView<VertexArray> loadMesh(Mesh& mesh, ShaderProgram& program);

    void renderImGui();

    void imGuiBeginPanel(const char* name) const;

    void imGuiEndPanel();

    void addImGuiDrawFunction(DrawFunction drawFunction);

    void imGuiDebugPanel(ResourceView<Context>&);

    void imGuiIndexBufferPanel(ResourceView<IndexBuffer>&);

    void imGuiVertexArrayPanel(ResourceView<VertexArray>&);

    void imGuiRenderTransformConstantBuffer(ResourceView<ConstantBuffer>& );

    void imGuiSamplerPanel(ResourceView<Sampler>&);

    void imGuiTextureParametersPanel(ResourceView<ConstantBuffer>& constantBuffer);

    void imGuiTexturePanel(ResourceView<Texture2D>& texture);

    void imGuiDepthTestPanel(ResourceView<FrameBuffer>& frameBuffer);

    void imGuiCameraPanel(Camera& camera);

    void imGuiFrameBufferPanel(ResourceView<FrameBuffer>& frameBuffer, int32& selectedTexture);

protected:
    ResourceView<Context>& m_Context;
    EasyIni::Configuration& m_Config;
    const Resource<SyriusWindow>& m_Window;
    ShaderLibrary m_ShaderLibrary;

    double m_DeltaTime;

private:
    std::vector<DrawFunction> m_ImGuiDraw;

    double m_LastFrameTime;

    float m_CurrentX = 0.0f;
    float m_CurrentY = 0.0f;
    float m_Column1Width = 200.0f;
    float m_Column2Width = 200.0f;
    float* m_CurrentColumnWidth = &m_Column1Width;

};
