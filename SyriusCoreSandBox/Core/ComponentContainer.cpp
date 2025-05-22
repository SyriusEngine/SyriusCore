#include "ComponentContainer.hpp"

#include "../ComponentLayers/Debug.hpp"
#include "../ComponentLayers/VertexDraw.hpp"
#include "../ComponentLayers/IndexDraw.hpp"
#include "../ComponentLayers/MVP.hpp"

#include "../Components/Camera.hpp"
#include "../Components/Projection.hpp"
#include "../Components/MeshComponent.hpp"
#include "../Components/ShaderComponent.hpp"

ComponentContainer::ComponentContainer(const UP<SyriusWindow>& window, const ResourceView<Context>& context):
m_Window(window),
m_Context(context){
    addFactory<CameraFactory>();
    addFactory<ProjectionFactory>();
    addFactory<ShaderComponentFactory>();
    addFactory<MeshComponentFactory>();

    addFactory<DebugFactory>();
    addFactory<VertexDrawFactory>();
    addFactory<IndexDrawFactory>();
    addFactory<MVPFactory>();
}

void ComponentContainer::createComponent(const std::string_view name){
    for (const auto& [provides, factory]: m_ComponentFactories) {
        if (provides == name) {
            // Found a factory that provides the requested component
            // First check if that factory also needs components
            const auto& requires = factory->requires();
            for (const auto& require: requires) {
                if (m_NameToComponent.find(require) == m_NameToComponent.end()) {
                    // Required component not created yet, so create it
                    createComponent(require);
                }
            }

            // Create the component
            auto component = factory->createComponent(m_Window, m_Context, this);
            m_Components.push_back(component);
            m_NameToComponent.insert(provides);
        }
    }
}

void ComponentContainer::onUpdate(const Duration deltaTime) const{
    for (const auto& component: m_Components){
        component->onUpdate(deltaTime);
    }
}

void ComponentContainer::onEvent(const Event &event) const{
    for (const auto& component: m_Components){
        component->onEvent(event);
    }
}

void ComponentContainer::onImGui() const{
    ImGuiWindowData data;
    for (const auto& component: m_Components){
        component->onImGui(data);
    }
}

