#include "MVP.hpp"

#include "../Components/ShaderComponent.hpp"
#include "../Core/ComponentContainer.hpp"

MVP::MVP(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    auto shader = container->getComponent<ShaderComponent>();
    shader->setShader("MVP", "MVP");
}
