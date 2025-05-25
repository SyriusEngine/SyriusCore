#include "MVPTextured.hpp"

#include "../Components/ShaderComponent.hpp"
#include "../Core/ComponentContainer.hpp"

MVPTextured::MVPTextured(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    const auto shader = container->getComponent<ShaderComponent>();
    shader->setShader("MVP", "Texture");
}
