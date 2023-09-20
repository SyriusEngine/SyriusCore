#include "Layer.hpp"

Layer::Layer(ResourceView<Context>& context, const Resource<SyriusWindow> &window, EasyIni::Configuration& config):
m_Context(context),
m_Window(window),
m_Config(config){

}

Layer::~Layer() {

}
