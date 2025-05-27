#include "MaterialComponent.hpp"

MaterialComponent::MaterialComponent(const UP<SyriusWindow> &window, const ResourceView<Context> &context, ComponentContainer *container):
IComponent(window, context, container){
    loadMaterial(0, "chipped-paint-metal");

    ConstantBufferDesc textureParamsDesc;
    TextureParameters params;
    textureParamsDesc.data = &params;
    textureParamsDesc.count = 1;
    textureParamsDesc.size = sizeof(TextureParameters);
    textureParamsDesc.shaderStage = SR_SHADER_FRAGMENT;
    textureParamsDesc.usage = SR_BUFFER_USAGE_DYNAMIC;
    textureParamsDesc.name = "TextureParameters";
    m_ParametersBuffer = m_Context->createConstantBuffer(textureParamsDesc);
}

void MaterialComponent::onUpdate(Duration deltaTime) {
    m_ParametersBuffer->bindShaderResource(4);

    const MaterialHandle& handle = m_Handles.at(m_SelectedMaterial);
    handle.albedo->bindShaderResource(0);
    handle.normal->bindShaderResource(1);
    handle.metallic->bindShaderResource(2);
    handle.roughness->bindShaderResource(3);
    handle.ao->bindShaderResource(4);
}

void MaterialComponent::onImGui(ImGuiWindowData &windowData) {
    imGuiBegin(windowData, "Material");
    if (ImGui::SliderFloat("Lerp Factor", &m_Parameters.lerpFactor, 0.0f, 1.0f)){
        m_ParametersBuffer->setData(&m_Parameters, sizeof(TextureParameters));
    }
    if (ImGui::SliderFloat("Scale", &m_Parameters.scale, 0.0f, 50.0f)){
        m_ParametersBuffer->setData(&m_Parameters, sizeof(TextureParameters));
    }

    imGuiEnd(windowData);
}

void MaterialComponent::loadMaterial(const i32 id, const std::string &name) {
    const fs::path basePath = fs::current_path() / "Resources" / "Textures" / name;

    MaterialHandle& handle = m_Handles[id];
    handle.albedo = loadTextureFromFile(basePath / std::string(name + "_basecolor.png"));
    handle.normal = loadTextureFromFile(basePath / std::string(name + "_normal.png"));
    handle.metallic = loadTextureFromFile(basePath / std::string(name + "_metallic.png"));
    handle.roughness = loadTextureFromFile(basePath / std::string(name + "_roughness.png"));
    handle.ao = loadTextureFromFile(basePath / std::string(name + "_ao.png"));
}

ResourceView<Texture2D> MaterialComponent::loadTextureFromFile(const fs::path &path) const {
    ImageFileDesc imageDesc;
    imageDesc.fileName = path;
    auto image = createImage(imageDesc);
    if (image->getChannelCount() == 3){
        image->extendAlpha();
    }
    Texture2DImageDesc desc;
    desc.image = createResourceView(image);
    auto texture = m_Context->createTexture2D(desc);
    return texture;
}




