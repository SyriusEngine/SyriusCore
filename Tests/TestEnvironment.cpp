#include "TestEnvironment.hpp"

Resource<SyriusWindow> TestEnvironment::m_Window;
ResourceView<Context> TestEnvironment::m_Context;
SR_SUPPORTED_API TestEnvironment::m_API = SR_API_NONE;
bool TestEnvironment::m_ExportImages = false;

void TestEnvironment::setup(const EnvironmentDesc &desc) {
    WindowDesc windowDesc;
    windowDesc.width = desc.width;
    windowDesc.height = desc.height;
    windowDesc.title = desc.title;
    m_Window = createWindow(windowDesc);

    ContextDesc contextDesc;
    contextDesc.api = desc.api;
    m_Context = m_Window->createContext(contextDesc);

    m_API = desc.api;
    m_ExportImages = desc.exportImages;
}

void TestEnvironment::teardown() {
    m_Window->destroyContext();
    m_Window.reset();
    m_Window = nullptr;
}
