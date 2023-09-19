#include "ShaderLibrary.hpp"

ShaderLibrary::ShaderLibrary(const std::string &libraryPath, const ResourceView<Context> &context):
m_Context(context),
m_LibraryPath(libraryPath) {

}

ShaderLibrary::~ShaderLibrary() {

}
