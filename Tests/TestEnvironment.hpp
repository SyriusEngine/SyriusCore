#pragma once

#include "TestInclude.hpp"

struct EnvironmentDesc{
    SR_SUPPORTED_API api;
    u32 width = 800;
    u32 height = 600;
    std::string title = "Test Window";
    bool exportImages = false;
};

class TestEnvironment{
public:
    static void setup(const EnvironmentDesc& desc);

    static void teardown();

    static ShaderModuleFileDesc createShaderModuleDescFromFileVS(const std::string &file);

    static ShaderModuleFileDesc createShaderModuleDescFromFileFS(const std::string &file);

public:
    static UP<SyriusWindow> m_Window;
    static ResourceView<Context> m_Context;
    static SR_SUPPORTED_API m_API;
    static bool m_ExportImages;
};

