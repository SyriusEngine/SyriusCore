#pragma once

#include "Include.hpp"

class ShaderLibrary{
public:
    ShaderLibrary(const std::string& libraryPath, const ResourceView<Context>& context);

    ~ShaderLibrary();

private:
    ResourceView<Context> m_Context;
    std::string m_LibraryPath;
    std::vector<Resource<Shader>> m_Shaders;
};