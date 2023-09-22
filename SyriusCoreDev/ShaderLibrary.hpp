#pragma once

#include "Utils.hpp"

class ShaderLibrary{
public:
    ShaderLibrary(const std::string& libraryPath, const ResourceView<Context>& context);

    ~ShaderLibrary();

    ShaderProgram loadShader(const std::string& vertexShader, const std::string& fragmentShader = "");

private:
    ResourceView<Context> m_Context;
    std::string m_LibraryPath;
    std::vector<Resource<Shader>> m_Shaders;
};