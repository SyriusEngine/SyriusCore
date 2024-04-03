#pragma once

#include "TestInclude.hpp"

struct EnvironmentDesc{
    SR_SUPPORTED_API api;
    uint32 width = 800;
    uint32 height = 600;
    std::string title = "Test Window";
};

class TestEnvironment{
public:
    static void setup(const EnvironmentDesc& desc);

    static void teardown();

public:
    static Resource<SyriusWindow> m_Window;
    static ResourceView<Context> m_Context;
};

