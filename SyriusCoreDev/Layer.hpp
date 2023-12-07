#pragma once

#include "Utils.hpp"
#include "ShaderLibrary.hpp"

class Layer{
public:
    Layer(ResourceView<Context>& context, const Resource<Window>& window, EasyIni::Configuration& config);

    virtual ~Layer();

    virtual void onAttach() = 0;

    virtual void onDetach() = 0;

    virtual void onUpdate() = 0;

    virtual void onEvent(const Event& event) = 0;

protected:

    ResourceView<VertexArray> loadMesh(Mesh& mesh, ShaderProgram& program);

protected:
    ResourceView<Context>& m_Context;
    EasyIni::Configuration& m_Config;
    const Resource<Window>& m_Window;
    ShaderLibrary m_ShaderLibrary;
};
