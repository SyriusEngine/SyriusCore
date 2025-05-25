#pragma once

#include "IComponent.hpp"

class ComponentContainer;

class IComponentFactory {
public:
    IComponentFactory() = default;

    virtual ~IComponentFactory() = default;

    virtual SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) = 0;

    virtual std::string provides() const = 0;

    virtual std::vector<std::string> requires() const = 0;
};
