#pragma once

#include "../Core/IComponentFactory.hpp"
#include "../Utils.hpp"

class ShaderComponent: public IComponent {
public:
    ShaderComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container);

    ~ShaderComponent() override = default;

    void onUpdate(Duration deltaTime) override;

    void setShader(const std::string& vertexShader, const std::string& fragmentShader);

    const ShaderProgram& getProgram() const { return m_Program; }

private:
    ShaderProgram m_Program;
};

class ShaderComponentFactory: public IComponentFactory {
public:
    ShaderComponentFactory() = default;
    ~ShaderComponentFactory() override = default;

    SP<IComponent> createComponent(const UP<SyriusWindow>& window, const ResourceView<Context>& context, ComponentContainer* container) override {
        return createSP<ShaderComponent>(window, context, container);
    }

    std::string provides() const override {
        return "ShaderComponent";
    }

    std::vector<std::string> requires() const override {
        return {};
    }
};