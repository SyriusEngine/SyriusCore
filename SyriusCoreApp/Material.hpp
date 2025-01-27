#pragma once

#include "Utils.hpp"

class Material{
public:
    Material(ResourceView<Context> &context, const std::string& materialName);

    ~Material();

    void bind(u32 baseIndex);

private:
    static ResourceView<Texture2D> loadTextureFromFile(ResourceView<Context>& context, const std::string& path);

private:
    ResourceView<Texture2D> m_Albedo;
    ResourceView<Texture2D> m_Normal;
    ResourceView<Texture2D> m_Metallic;
    ResourceView<Texture2D> m_Roughness;
    ResourceView<Texture2D> m_AO;
};