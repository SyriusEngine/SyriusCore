#include "TextureComponent.hpp"

#include "../Components/ShaderComponent.hpp"
#include "../Core/ComponentContainer.hpp"

TextureComponent::TextureComponent(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    ConstantBufferDesc textureParamsDesc;
    TextureParameters params;
    textureParamsDesc.data = &params;
    textureParamsDesc.size = sizeof(TextureParameters);
    textureParamsDesc.shaderStage = SR_SHADER_FRAGMENT;
    textureParamsDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    textureParamsDesc.name = "TextureParameters";
    m_ParametersBuffer = m_Context->createConstantBuffer(textureParamsDesc);

    ImageFileDesc awesomeFaceImageDesc;
    awesomeFaceImageDesc.fileName = "./Resources/Textures/awesomeface.png";
    awesomeFaceImageDesc.flipOnAccess = true;
    awesomeFaceImageDesc.requestedChannelCount = 4;
    const auto awesomeFace = createImage(awesomeFaceImageDesc);
    Texture2DImageDesc tex1Desc;
    tex1Desc.usage = SR_BUFFER_USAGE_DEFAULT;
    tex1Desc.image = createResourceView(awesomeFace);
    m_AwesomeFace = m_Context->createTexture2D(tex1Desc);

    ImageFileDesc InstagramImageDesc;
    InstagramImageDesc.fileName = "./Resources/Textures/insta512.png";
    InstagramImageDesc.flipOnAccess = true;
    const auto instagram = createImage(InstagramImageDesc);
    Texture2DImageDesc tex2Desc;
    tex2Desc.image = createResourceView(instagram);
    tex2Desc.usage = SR_BUFFER_USAGE_DEFAULT;
    m_Instagram = m_Context->createTexture2D(tex2Desc);
}

void TextureComponent::onUpdate(Duration deltaTime) {
    m_ParametersBuffer->bindShaderResource(4);
    m_AwesomeFace->bindShaderResource(0);
    m_Instagram->bindShaderResource(1);
}

void TextureComponent::onImGui(ImGuiWindowData &windowData) {
    imGuiBegin(windowData, "Texture");
    if (ImGui::SliderFloat("Lerp Factor", &m_Parameters.lerpFactor, 0.0f, 1.0f)){
        m_ParametersBuffer->setData(&m_Parameters, sizeof(TextureParameters));
    }
    if (ImGui::SliderFloat("Scale", &m_Parameters.scale, 0.0f, 50.0f)){
        m_ParametersBuffer->setData(&m_Parameters, sizeof(TextureParameters));
    }

    imGuiEnd(windowData);
}


