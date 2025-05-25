#include "ShaderComponent.hpp"

ShaderComponent::ShaderComponent(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    /*
     * D3D11 Expects a shader to be bound to create the vertex array. Since the vertex layout does not change
     * between all components, loading any shader (even if it is not used) is fine to get the input layout object
     */
    setShader("VertexBuffer", "VertexBuffer");

}

void ShaderComponent::onUpdate(Duration deltaTime) {
    SR_PRECONDITION(m_Program.shaderProgram != nullptr, "Shader not set!")

    m_Program.shaderProgram->bind();
}

void ShaderComponent::setShader(const std::string &vertexShader, const std::string &fragmentShader) {
    m_Program = loadShader(m_Context, vertexShader, fragmentShader);
}
