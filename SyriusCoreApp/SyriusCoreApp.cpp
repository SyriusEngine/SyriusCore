#include "SyriusCoreApp.hpp"

SyriusCoreApp::SyriusCoreApp(const std::string& configFile, const std::string& windowTitle):
m_Config(configFile){
    ContextDesc cDesc;
    cDesc.api = static_cast<SR_SUPPORTED_API>(m_Config["Context"]["API"].getOrDefault<u32>(SR_API_OPENGL));
    cDesc.enableDepthTest = m_Config["Context"]["EnableDepthTest"].getOrDefault<bool>(false);

    std::string windowName = windowTitle;
    if (cDesc.api == SR_API_OPENGL){
        windowName += " (OpenGL)";
    }
    else if (cDesc.api == SR_API_D3D11){
        windowName += " (DirectX 11)";
    }
    else{
        throw std::runtime_error("SyriusCoreApp::SyriusCoreApp: Unsupported API");
    }
    WindowDesc wDesc;
    wDesc.title = windowName;
    wDesc.width = m_Config["Window"]["Width"].getOrDefault<u32>(1280);
    wDesc.height = m_Config["Window"]["Height"].getOrDefault<u32>(720);

    m_Window = createWindow(wDesc);

    m_Context = m_Window->createContext(cDesc);
    auto vsync = m_Config["Context"]["VerticalSync"].getOrDefault<bool>(true);
    if (vsync){
        m_Context->setVerticalSynchronisation(true);
    }
    printContextInfo(m_Context);
    m_EnableImGui = m_Config["Window"]["EnableImGui"].getOrDefault<bool>(true);
    if (m_EnableImGui){
        m_Window->createImGuiContext();
    }
}

SyriusCoreApp::~SyriusCoreApp() {
    m_Components.clear();
    m_Config.save();
    m_Window->destroyContext();
}

void SyriusCoreApp::run() {
    while (m_Window->isOpen()){
        processEvents();

        m_Context->clear();
        m_Context->beginRenderPass();
        for (const auto& component: m_Components) {
            component->onRender();
        }
        m_Context->endRenderPass();

        if (m_EnableImGui){
            m_Context->beginRenderPass();
            ImGuiSizeData sizeData;
            m_Window->onImGuiBegin();
            for (const auto& component: m_Components) {
                component->onImGuiRender(sizeData);
            }
            m_Window->onImGuiEnd();
            m_Context->endRenderPass();
        }

        m_Context->swapBuffers();
    }
}

void SyriusCoreApp::processEvents() {
    m_Window->pollEvents();
    while (m_Window->hasEvent()){
        auto event = m_Window->getNextEvent();
        if (m_Config["Window"]["PrintEventInfo"].getOrDefault<bool>(false)){
            printEventInfo(event);
        }
        switch (event.type) {
            case SR_EVENT_WINDOW_CLOSED:
                m_Window->close();
                break;
            case SR_EVENT_WINDOW_RESIZED:
                m_Context->onResize(event.windowWidth, event.windowHeight);
                break;
            default:
                break;
        }
        for (const auto& component: m_Components){
            component->onEvent(event);
        }
    }
}