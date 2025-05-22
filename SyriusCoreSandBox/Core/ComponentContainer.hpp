#pragma once

#include <unordered_set>

#include "../SandBoxInclude.hpp"

#include "IComponent.hpp"
#include "IComponentFactory.hpp"

class ComponentContainer {
public:
    ComponentContainer(const UP<SyriusWindow>& window, const ResourceView<Context>& context);

    ~ComponentContainer() = default;

    void createComponent(std::string_view);

    void onUpdate(Duration deltaTime) const;

    void onEvent(const Event& event) const;

    void onImGui() const;

    template<typename COMPONENT>
    SP<COMPONENT> getComponent() const {
        static_assert(std::is_base_of_v<IComponent, COMPONENT>, "COMPONENT type is not derived from IComponent");
        for (const auto& component : m_Components) {
            if (auto casted = std::dynamic_pointer_cast<COMPONENT>(component)) {
                return casted;
            }
        }
        throw std::runtime_error("Component not found");
    }

private:

    template<typename FACTORY>
    void addFactory() {
        static_assert(std::is_base_of_v<IComponentFactory, FACTORY>, "FACTORY type is not derived from IFactoryComponent");

        auto factory = createSP<FACTORY>();
        m_ComponentFactories.insert({factory->provides(), factory});
    }

private:
    const UP<SyriusWindow>& m_Window;
    const ResourceView<Context>& m_Context;

    std::vector<SP<IComponent>> m_Components;
    std::unordered_map<std::string, SP<IComponentFactory>> m_ComponentFactories;
    std::unordered_set<std::string> m_NameToComponent;
};
