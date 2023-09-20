#pragma once

#include "Utils.hpp"

class Layer{
public:
    Layer(ResourceView<Context>& context, const Resource<SyriusWindow>& window, EasyIni::Configuration& config);

    virtual ~Layer();

    virtual void onAttach() = 0;

    virtual void onDetach() = 0;

    virtual void onUpdate() = 0;

    virtual void onEvent(const Event& event) = 0;

protected:
    ResourceView<Context>& m_Context;
    EasyIni::Configuration& m_Config;
    const Resource<SyriusWindow>& m_Window;
};
