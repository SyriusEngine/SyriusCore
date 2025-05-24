#include "ShaderComponent.hpp"

ShaderComponent::ShaderComponent(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    setShader("MVP", "MVP");

}

void ShaderComponent::onUpdate(Duration deltaTime) {
    SR_PRECONDITION(m_Program.shaderProgram != nullptr, "Shader not set!")

    m_Program.shaderProgram->bind();
}

void ShaderComponent::setShader(const std::string &vertexShader, const std::string &fragmentShader) {
    m_Program = loadShader(m_Context, vertexShader, fragmentShader);
}
