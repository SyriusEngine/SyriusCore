#include "PBRLayer.hpp"

#include "../Components/ShaderComponent.hpp"
#include "../Core/ComponentContainer.hpp"

PBRLayer::PBRLayer(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    const auto shader = container->getComponent<ShaderComponent>();
    shader->setShader("GeometryPass", "GeometryPass");
}

