#include "Material.hpp"

Material::Material(ResourceView<Context> &context, const std::string &materialName) {
    const std::string baseResourcePath = "Resources/Textures/" + materialName + "/" + materialName;
    m_Albedo = loadTextureFromFile(context, baseResourcePath + "_basecolor.png");
    m_Normal = loadTextureFromFile(context, baseResourcePath + "_normal.png");
    m_Metallic = loadTextureFromFile(context, baseResourcePath + "_metallic.png");
    m_Roughness = loadTextureFromFile(context, baseResourcePath + "_roughness.png");
    m_AO = loadTextureFromFile(context, baseResourcePath + "_ao.png");
}

Material::~Material() = default;

void Material::bind(u32 baseIndex) {
    m_Albedo->bindShaderResource(baseIndex);
    m_Normal->bindShaderResource(baseIndex + 1);
    m_Metallic->bindShaderResource(baseIndex + 2);
    m_Roughness->bindShaderResource(baseIndex + 3);
    m_AO->bindShaderResource(baseIndex + 4);
}

ResourceView<Texture2D> Material::loadTextureFromFile(ResourceView<Context> &context, const std::string &path) {
    ImageFileDesc imageDesc;
    imageDesc.fileName = path;
    auto image = createImage(imageDesc);
    if (image->getChannelCount() == 3){
        image->extendAlpha();
    }
    Texture2DImageDesc desc;
    desc.image = createResourceView(image);
    auto texture = context->createTexture2D(desc);
    return texture;
}
