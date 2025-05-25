#include "ComponentContainer.hpp"

#include "../ComponentLayers/Debug.hpp"
#include "../ComponentLayers/VertexDraw.hpp"
#include "../ComponentLayers/IndexDraw.hpp"
#include "../ComponentLayers/MVP.hpp"
#include "../ComponentLayers/MVPTextured.hpp"
#include "../ComponentLayers/FrameBufferLayer.hpp"
#include "../ComponentLayers/CubeMapLayer.hpp"
#include "../ComponentLayers/DeferredPBRLayer.hpp"
#include "../ComponentLayers/PBRLayer.hpp"
#include "../ComponentLayers/ImGuiLayer.hpp"

#include "../Components/Camera.hpp"
#include "../Components/ProjectionComponent.hpp"
#include "../Components/MeshComponent.hpp"
#include "../Components/ShaderComponent.hpp"
#include "../Components/DepthTest.hpp"
#include "../Components/SamplerComponent.hpp"
#include "../Components/TextureComponent.hpp"
#include "../Components/MaterialComponent.hpp"

ComponentContainer::ComponentContainer(const UP<SyriusWindow>& window, const ResourceView<Context>& context):
m_Window(window),
m_Context(context){
    addFactory<CameraFactory>();
    addFactory<ProjectionFactory>();
    addFactory<ShaderComponentFactory>();
    addFactory<MeshComponentFactory>();
    addFactory<DepthTestFactory>();
    addFactory<SamplerFactory>();
    addFactory<TextureFactory>();
    addFactory<MaterialFactory>();

    addFactory<DebugFactory>();
    addFactory<VertexDrawFactory>();
    addFactory<IndexDrawFactory>();
    addFactory<MVPFactory>();
    addFactory<MVPTexturedFactory>();
    addFactory<FrameBufferLayerFactory>();
    addFactory<CubeMapFactory>();
    addFactory<PBRFactory>();
    addFactory<DeferredPBRFactory>();
    addFactory<ImGuiFactory>();
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

void ComponentContainer::onImGui(ImGuiWindowData& windowData) const{
    for (const auto& component: m_Components){
        component->onImGui(windowData);
    }
}

