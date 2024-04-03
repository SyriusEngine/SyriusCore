#include "TestEnvironment.hpp"

Resource<SyriusWindow> TestEnvironment::m_Window;
ResourceView<Context> TestEnvironment::m_Context;

void TestEnvironment::setup(const EnvironmentDesc &desc) {
    WindowDesc windowDesc;
    windowDesc.width = desc.width;
    windowDesc.height = desc.height;
    windowDesc.title = desc.title;
    m_Window = createWindow(windowDesc);

    ContextDesc contextDesc;
    contextDesc.api = desc.api;
    m_Context = m_Window->createContext(contextDesc);
}

void TestEnvironment::teardown() {
    m_Window->destroyContext();
    m_Window.reset();
    m_Window = nullptr;
}
